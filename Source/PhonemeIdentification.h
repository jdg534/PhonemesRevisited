#pragma once

#include <map>
#include <vector>
#include <string>

#include <nlohmann/json.hpp>

class PhonemeIdentification
{
public:

	void Initialise(const std::string& file_path);

	bool IsInitialised() const;


private:

	void InitialiseAnalysis(const nlohmann::json& analysisNode);
	void InitialiseComparisonData(const nlohmann::json& phonemesNode);

	const std::map<std::string, std::vector<float>>& ComparisonData() const;
	const std::vector<float>& TargetFrequencies() const;
	unsigned int MovingAvarageFilterNumberOfSamples() const;

	std::map<std::string, std::vector<float>> m_ComparisonData;
	std::vector<float> m_TargetFrequencies;
	unsigned int m_MovingAvarageFilterNumberOfSamples { 1 };
};
