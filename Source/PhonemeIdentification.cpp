#include "PhonemeIdentification.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <bitset>

#include <SDL3/SDL.h>

#include <SignalAnalysis.h>

void PhonemeIdentification::Initialise(const std::string& file_path)
{
	std::ifstream InputFile = std::ifstream(file_path);
	if (!InputFile.good()) { return; }
	m_PhonemesRootDirectory = file_path.substr(0, file_path.find_first_of('/') + 1);
	const nlohmann::json Config = nlohmann::json::parse(InputFile);
	if (Config.contains("analysis")) { InitialiseAnalysis(Config["analysis"]); }
	if (Config.contains("phonemes")) { InitialiseComparisonData(Config["phonemes"]); }
}

bool PhonemeIdentification::IsInitialised() const
{
	return !ComparisonData().empty()
		&& !TargetFrequencies().empty()
		&& !m_PhonemesRootDirectory.empty();
}

bool PhonemeIdentification::IsPhonemePresent(const std::string& phoneticSymbol) const
{
	return ComparisonData().find(phoneticSymbol) != ComparisonData().cend();
}

void PhonemeIdentification::InitialiseAnalysis(const nlohmann::json& analysisNode)
{
	/* Structure: { "moving_avarage_filter" : 5, "target_frequencies" : [ 80.0, 85.0]}*/
	if (analysisNode.contains("moving_avarage_filter"))
	{
		m_MovingAvarageFilterNumberOfSamples = analysisNode["moving_avarage_filter"];
	}
	assert(analysisNode.contains("target_frequencies"));
	const size_t NumTargetFrequencies = analysisNode["target_frequencies"].size();
	m_TargetFrequencies.resize(NumTargetFrequencies);
	for (size_t i = 0; i < NumTargetFrequencies; ++i)
	{
		m_TargetFrequencies[i] = analysisNode["target_frequencies"][i];
	}
}

void PhonemeIdentification::InitialiseComparisonData(const nlohmann::json& phonemesNode)
{
	using namespace std;
	assert(!TargetFrequencies().empty());
	assert(phonemesNode.is_array());
	/* structure: "phonemes" : [ { "symbol" : "a", "file_path" : "English/a.wav" },
	{ "symbol" : "ar", "file_path" : "English/ar.wav" }]*/
	const auto& TargetFrequenciesRef = TargetFrequencies();
	const size_t NumTargetFrequencies = TargetFrequenciesRef.size();

	SDL_AudioSpec SpecToAnalyse;
	for (const nlohmann::json& PhonemeNode : phonemesNode)
	{
		const string PhonemeSymbol = PhonemeNode["symbol"];
		const string FilePath = PhonemesRootDirectory() + string(PhonemeNode["file_path"]);

		// Get SDL to load the waveform (float format), remember that these are relative file paths.
		Uint8* AudioBuffer = nullptr;
		Uint32 AudioBufferLength = 0;
		SDL_memset(&SpecToAnalyse, 0, sizeof(SDL_AudioSpec));
		if (SDL_LoadWAV(FilePath.c_str(), &SpecToAnalyse, &AudioBuffer, &AudioBufferLength) == nullptr)
		{
			throw std::runtime_error("Failed to load file");
		}
		if (SpecToAnalyse.channels != 1) { throw std::runtime_error("Only mono audio comparison data is accepted"); }
		// Tried SDL_ConvertAudio() didn't work as expected. 
		switch (SpecToAnalyse.format)
		{
			case AUDIO_U8: AddComparisonData(PhonemeSymbol, AudioBuffer, AudioBufferLength); break;
			case AUDIO_S8: AddComparisonData(PhonemeSymbol, reinterpret_cast<Sint8*>(AudioBuffer), AudioBufferLength); break;
			case AUDIO_U16: AddComparisonData(PhonemeSymbol, reinterpret_cast<Uint16*>(AudioBuffer), AudioBufferLength / 2); break;
			case AUDIO_S16: AddComparisonData(PhonemeSymbol, reinterpret_cast<Sint16*>(AudioBuffer), AudioBufferLength / 2); break;
			case AUDIO_S32: AddComparisonData(PhonemeSymbol, reinterpret_cast<Sint32*>(AudioBuffer), AudioBufferLength / 4); break;
			case AUDIO_F32: AddComparisonData(PhonemeSymbol, reinterpret_cast<float*>(AudioBuffer), AudioBufferLength / 4); break;
			default: break;
		}
		SDL_FreeWAV(AudioBuffer);
		AudioBuffer = nullptr;
		AudioBufferLength = 0;
		SDL_memset(&SpecToAnalyse, 0, sizeof(SDL_AudioSpec));
	}
}

void PhonemeIdentification::AddComparisonData(const std::string& phoneticSymbol, const Uint8* waveform, const size_t numWaveformSamples)
{
	const size_t MemNeededToStoreAsFloat = numWaveformSamples * 4;
	float* FloatRepresentation = new float[numWaveformSamples];
	std::memset(FloatRepresentation, 0, MemNeededToStoreAsFloat);
	Signal::Conversion::u8ToF(waveform, numWaveformSamples, FloatRepresentation);
	AddComparisonData(phoneticSymbol, FloatRepresentation, numWaveformSamples);
	delete[] FloatRepresentation;
}

void PhonemeIdentification::AddComparisonData(const std::string& phoneticSymbol, const Sint8* waveform, const size_t numWaveformSamples)
{
	const size_t MemNeededToStoreAsFloat = numWaveformSamples * 4;
	float* FloatRepresentation = new float[numWaveformSamples];
	std::memset(FloatRepresentation, 0, MemNeededToStoreAsFloat);
	Signal::Conversion::i8ToF(waveform, numWaveformSamples, FloatRepresentation);
	AddComparisonData(phoneticSymbol, FloatRepresentation, numWaveformSamples);
	delete[] FloatRepresentation;
}

void PhonemeIdentification::AddComparisonData(const std::string& phoneticSymbol, const Uint16* waveform, const size_t numWaveformSamples)
{
	const size_t MemNeededToStoreAsFloat = numWaveformSamples * 4;
	float* FloatRepresentation = new float[numWaveformSamples];
	std::memset(FloatRepresentation, 0, MemNeededToStoreAsFloat);
	Signal::Conversion::u16ToF(waveform, numWaveformSamples, FloatRepresentation);
	AddComparisonData(phoneticSymbol, FloatRepresentation, numWaveformSamples);
	delete[] FloatRepresentation;
}

void PhonemeIdentification::AddComparisonData(const std::string& phoneticSymbol, const Sint16* waveform, const size_t numWaveformSamples)
{
	const size_t MemNeededToStoreAsFloat = numWaveformSamples * 4;
	float* FloatRepresentation = new float[numWaveformSamples];
	std::memset(FloatRepresentation, 0, MemNeededToStoreAsFloat);
	Signal::Conversion::i16ToF(waveform, numWaveformSamples, FloatRepresentation);
	AddComparisonData(phoneticSymbol, FloatRepresentation, numWaveformSamples);
	delete[] FloatRepresentation;
}

void PhonemeIdentification::AddComparisonData(const std::string& phoneticSymbol, const Sint32* waveform, const size_t numWaveformSamples)
{
	const size_t MemNeededToStoreAsFloat = numWaveformSamples * 4;
	float* FloatRepresentation = new float[numWaveformSamples];
	std::memset(FloatRepresentation, 0, MemNeededToStoreAsFloat);
	Signal::Conversion::i32ToF(waveform, numWaveformSamples, FloatRepresentation);
	AddComparisonData(phoneticSymbol, FloatRepresentation, numWaveformSamples);
	delete[] FloatRepresentation;
}

void PhonemeIdentification::AddComparisonData(const std::string& phoneticSymbol, const float* waveform, const size_t numWaveformSamples)
{
	using namespace std;
	const auto& ComparisonDataRef = ComparisonData();
	if (ComparisonDataRef.find(phoneticSymbol) != ComparisonDataRef.cend()) { throw runtime_error("duplicate phonetic symbol: " + phoneticSymbol); }
	const auto& TargetFrequenciesRef = TargetFrequencies();
	if (TargetFrequenciesRef.empty()) { throw runtime_error("no target frequencies"); }
	// -1.0f to 1.0f range expected.
	const size_t NumTargetFrequencies = TargetFrequenciesRef.size();
	float* TargetFrequencies = new float[NumTargetFrequencies];
	Signal::FourierTransforms::DiscreteFourierTransformFullTargetFrequenciesF(waveform, numWaveformSamples,
		NumTargetFrequencies, &TargetFrequenciesRef[0], TargetFrequencies);
	std::vector<float> TargetFrequenciesOutput(NumTargetFrequencies);
	std::memcpy(&TargetFrequenciesOutput[0], TargetFrequencies, NumTargetFrequencies * sizeof(float));
	m_ComparisonData.emplace(phoneticSymbol, TargetFrequenciesOutput);
	delete[] TargetFrequencies;
}

const std::string& PhonemeIdentification::PhonemesRootDirectory() const
{
	return m_PhonemesRootDirectory;
}

const std::map<std::string, std::vector<float>>& PhonemeIdentification::ComparisonData() const
{
	return m_ComparisonData;
}

const std::vector<float>& PhonemeIdentification::TargetFrequencies() const
{
	return m_TargetFrequencies;
}

unsigned int PhonemeIdentification::MovingAvarageFilterNumberOfSamples() const
{
	return m_MovingAvarageFilterNumberOfSamples;
}