/*
  ==============================================================================

    MyReverb.cpp
    Created: 10 Oct 2021 9:53:28pm
    Author:  97252

  ==============================================================================
*/

#include "MyReverb.h"


//==============================================================================
MyReverb::MyReverb() : num_delay_lines(12),
						b_gain(1.0),
						balance_dry(1.0),
						feedback_matrix(12, 12),
						delay_line_lengths{ 601, 1399, 1747, 2269, 2707, 3089, 3323, 3571, 3911, 4127, 4639, 4999 },
						is_clipping(false),
						input_gain(0.0),
						output_gain(0.0),
						input_buffer_size(0),
						balance_current_value(0.0),
						time_current_value(1.5),
						dampening_current_value(75.0),
						init_current_value(0.0)
{
			setupGainC(0.0);
		
			dsp::Matrix <float> toeplitzVector(12, 1);
			dsp::Matrix <float> circulantMatrix(12, 12);
			dsp::Matrix <float> uVector(12, 1);
			dsp::Matrix <float> uVectorTransposed(1, 12);
		
			toeplitzVector.clear();
			toeplitzVector(1, 0) = 1.0;

			circulantMatrix = circulantMatrix.toeplitz(toeplitzVector, 12);
			for (int cnt = 0; cnt < 11; ++cnt)
			{
				circulantMatrix(cnt, cnt + 1) = 0.0;
			}
			circulantMatrix(0, 11) = 1.0;
			for (int cnt = 0; cnt < 12; ++cnt)
			{
				uVector(cnt, 0) = 1.0;
				uVectorTransposed(0, cnt) = 1.0;
			}
			feedback_matrix = circulantMatrix - ((uVector * (2.0 / num_delay_lines)) * uVectorTransposed);
		
			for (int cnt = 0; cnt < 12; ++cnt)
				feedback_matrix_rows.add(new dsp::Matrix <float>(1, 12, feedback_matrix.getRawDataPointer() + cnt * 12));
		
			input_buffer.clear();
			output_buffer.clear();
			dampening_filters_tab.clear();
			dampening_filters_coeffs_tab.clear();
}

MyReverb::~MyReverb()
{
}

bool MyReverb::getIsClipping()
{
	return is_clipping;
}

void MyReverb::setBalanceCurrentValue(float balanceValue)
{
	balance_current_value = balanceValue;
}

void MyReverb::setTimeCurrentValue(float timeValue)
{
	time_current_value = timeValue;
}

void MyReverb::setDampeningCurrentValue(float dampeningValue)
{
	dampening_current_value = dampeningValue;
}

void MyReverb::setInitCurrentValue(float initValue)
{
	init_current_value = initValue;
}

void MyReverb::setInputGain(float inGainChosen)
{
	input_gain = inGainChosen;
}

void MyReverb::setOutputGain(float outGainChosen)
{
	output_gain = outGainChosen;
}

void MyReverb::setInputBuffer(AudioBuffer <float>& newFileBuffer)
{
	input_buffer.makeCopyOf(newFileBuffer);
	input_buffer_size = input_buffer.getNumSamples();
	is_clipping = false;
}

void MyReverb::setupMyReverb()
{

	output_buffer.setSize(input_buffer.getNumChannels(),
		input_buffer_size
		+ int(44100.0 * time_current_value)
		+ int(std::ceil(44.1 * init_current_value))
		+ 11025,
		false,
		true,
		false);
	input_buffer.setSize(input_buffer.getNumChannels(),
		output_buffer.getNumSamples(),
		true,
		true,
		false);

	b_gain = Decibels::decibelsToGain(input_gain);
	output_gain = Decibels::decibelsToGain(output_gain);

	if (balance_current_value <= 0.0)
	{
		setupGainC(std::pow(balance_current_value + 1.0, 4));
		balance_dry = 1.0;
	}
	else if (balance_current_value > 0.0)
	{
		setupGainC();
		balance_dry = std::pow(1.0 - balance_current_value, 4);
	}

	dampening_filters_tab.clear();
	dampening_filters_coeffs_tab.clear();
	tonal_correction_filter_ptr.clear();
	tonal_correction_filter_coeffs_ptr.reset();

	float minAlphaConst = (4.0 * time_current_value) / (-3.0 * 4999.0 * (1.0 / 44100.0) * std::log(10.0));
	float minAlpha = std::sqrt(1.0 / (1.0 - minAlphaConst));
	float alpha = minAlpha + ((100.0 - dampening_current_value) * ((1.0 - minAlpha) / 100.0));

	float gCoeff = 0.0;
	float pCoeff = 0.0;
	float constElement1 = std::log(10.0) / 4.0;
	float constElement2 = 1.0 - (1.0 / std::pow(alpha, 2));

	for (int cnt = 0; cnt < 12; ++cnt)
	{
		gCoeff = std::pow(10.0, (-3.0 * delay_line_lengths[cnt] * (1.0 / 44100.0)) / time_current_value);
		pCoeff = constElement1 * constElement2 * std::log10(gCoeff);
		dampening_filters_coeffs_tab.add(new dsp::IIR::Coefficients <float>(gCoeff * (1.0 - pCoeff), 0.0, 1.0, -pCoeff));
		dampening_filters_tab.add(new dsp::IIR::Filter <float>(dampening_filters_coeffs_tab.getLast()));
	}

	float beta = (1.0 - alpha) / (1.0 + alpha);
	tonal_correction_filter_coeffs_ptr = new dsp::IIR::Coefficients <float>(1.0, -beta, 1.0 - beta, 0.0);
	tonal_correction_filter_ptr.set(new dsp::IIR::Filter <float>(tonal_correction_filter_coeffs_ptr.get()), true);

	is_clipping = false;
}

AudioBuffer <float>& MyReverb::addReverb()
{
	ScopedNoDenormals noDenormals;

	int initDelayLineLength = 1;

	if (init_current_value != 0.0)
		initDelayLineLength = int(std::round(init_current_value * 44.1));

	std::deque <float> initDelayLine(initDelayLineLength, 0.0);
	OwnedArray <std::deque <float>> delayLines;
	dsp::Matrix <float> tempMatrix(12, 1);
	float outSample = 0.0;
	float currentSample = 0.0;
	dsp::Matrix <float> currentMatrixProduct(1, 1);

	for (int cnt = 0; cnt < 12; ++cnt)
		delayLines.add(new std::deque <float>(4999, 0.0));

	for (auto channel = 0; channel < input_buffer.getNumChannels(); ++channel)
	{
		float* outputWrite = output_buffer.getWritePointer(channel);
		const float* inputRead = input_buffer.getReadPointer(channel);

		for (auto sample = 0; sample < output_buffer.getNumSamples(); ++sample)
		{
			outSample = 0.0;
			for (int cnt = 0; cnt < 12; ++cnt)
			{
				tempMatrix(cnt, 0) = (*delayLines[cnt])[delay_line_lengths[cnt] - 1];
				outSample += tempMatrix(cnt, 0) * c_gain[cnt][channel];

				currentMatrixProduct = *feedback_matrix_rows[cnt] * tempMatrix;
				currentSample = (inputRead[sample] * b_gain) + *(currentMatrixProduct.getRawDataPointer());
				currentSample = dampening_filters_tab[cnt]->processSample(currentSample);
				delayLines[cnt]->push_front(currentSample);
				delayLines[cnt]->pop_back();
			}

			outSample = tonal_correction_filter_ptr.get()->processSample(outSample);
			initDelayLine.push_front(outSample);
			initDelayLine.pop_back();
			outputWrite[sample] = ((inputRead[sample] * b_gain * balance_dry)
				+ initDelayLine.back()) * output_gain;

			if (is_clipping == false)
			{
				if (outputWrite[sample] > 1.0 || outputWrite[sample] < -1.0)
					is_clipping = true;
			}
		}

		for (int cnt = 0; cnt < 12; ++cnt)
			dampening_filters_tab[cnt]->reset();
		tonal_correction_filter_ptr->reset();
	}

	auto a = output_buffer.getMagnitude(0, output_buffer.getNumSamples());
	return output_buffer;
}

void MyReverb::setupGainC(float coeff)
{
	unsigned short flag = 1;
	for (int cnt1 = 0; cnt1 < 12; ++cnt1)
	{
		for (int cnt2 = 0; cnt2 < 2; ++cnt2)
		{
			if (flag % 3 != 0 && flag != 7 && flag != 8)
				c_gain[cnt1][cnt2] = 1.0 * coeff;
			else
				c_gain[cnt1][cnt2] = -1.0 * coeff;
			++flag;
			if (flag == 9) flag = 1;
		}
	}
}


