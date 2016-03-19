function cepstrum_coefficient = calc_mfcc(data)
	spectrum = fft(data);
	magnitude = abs(spectrum);
	power_spectrum = magnitude.^2;

	energy_melband = mel_filter(power_spectrum);
	cepstrum_coefficient = discrete_cosine_transform(energy_melband);
end
