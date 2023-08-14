#include "PhonemeIdentification.h"

#include <fstream>

#include <SDL3/SDL.h>

// #include <Signal.h> refactor to <SignalAnalysis.h> get's mixed up with <signal.h> from STL.

void PhonemeIdentification::Initialise(const std::string& file_path)
{

	std::ifstream InputFile = std::ifstream(file_path);
	if (!InputFile.good()) { return; }
	const nlohmann::json Config = nlohmann::json::parse(InputFile);
	if (Config.contains("analysis")) { InitialiseAnalysis(Config["analysis"]); }
	if (Config.contains("phonemes")) { InitialiseComparisonData(Config["phonemes"]); }
}

bool PhonemeIdentification::IsInitialised() const
{
	return !ComparisonData().empty()
		&& !TargetFrequencies().empty();
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

	for (const nlohmann::json& PhonemeNode : phonemesNode)
	{
		const string PhonemeSymbol = PhonemeNode["symbol"];
		const string FilePath = PhonemeNode["file_path"];

		// Get SDL to load the waveform (float format), remember that these are relative file paths.
	}

	/*
	SDL_AudioSpec AudioSpec;
	SDL_memset(&AudioSpec, 0, sizeof(SDL_AudioSpec));
	SDL_GetAudioDeviceSpec(0, 0, &AudioSpec);
	SDL_LoadWAV("", &AudioSpec, nullptr, nullptr);
	*/
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