#include "main.h"

#include "VisemeAnimatedMesh.h"

#include <fstream>

VisemeAnimatedMesh::VisemeAnimatedMesh(const PhonemeIdentification* phonemeIdentifier)
: m_PhonemeIdentifier (phonemeIdentifier)
{
}

void VisemeAnimatedMesh::Initialise(const std::string& filePath)
{
	std::ifstream InputFile = std::ifstream(filePath);
	if (!InputFile.good()) { return; }
	m_VisemeRootDirectory = filePath.substr(0, filePath.find_first_of('/') + 1);
	const nlohmann::json Config = nlohmann::json::parse(InputFile);
	if (Config.contains("visemes")) { InitialiseVisemes(Config["visemes"]); }
	if (Config.contains("phoneme_associations")) { InitialisePhonemeAssociations(Config["phoneme_associations"]); }
	if (Config.contains("animation_settings")) { InitialiseAnimationSettings(Config["animation_settings"]); }
}

bool VisemeAnimatedMesh::IsInitialised() const
{
	return !VisemeRootDirectory().empty()
		&& !Visemes().empty()
		&& !PhonemeToVisemeAssociations().empty()
		&& !m_StateToDisplay.Vertices.empty();
}

void VisemeAnimatedMesh::InitialiseVisemes(const nlohmann::json& visemesNode)
{
	using namespace std;
	//visemesNode will be an array of: { "symbol" : "default", "file_path" : "Silence.viseme" }
	for (const nlohmann::json& VisemeEntry : visemesNode)
	{
		const string VisemeFilePath = VisemeRootDirectory() + string(VisemeEntry["file_path"]);
		AddViseme(VisemeEntry["symbol"], VisemeFilePath);
	}
	const auto DefaultViseme = Visemes().find("default");
	if (DefaultViseme == Visemes().cend()) { throw runtime_error("No default Viseme present"); }
	m_StateToDisplay.Vertices = DefaultViseme->second.Vertices;
}

void VisemeAnimatedMesh::InitialisePhonemeAssociations(const nlohmann::json& associationsNode)
{
	using namespace std;
	// associationsNode will be an array of: { "phoneme_symbol" : "a", "viseme_symbol" : "default" }
	for (const nlohmann::json& association : associationsNode)
	{
		const string PhonemeSymbol = association["phoneme_symbol"];
		const string VisemeSymbol = association["viseme_symbol"];
		if (!m_PhonemeIdentifier->IsPhonemePresent(PhonemeSymbol)) { throw runtime_error("Unsupported Phonetic symbol present"); }
		if (PhonemeToVisemeAssociations().find(PhonemeSymbol) != PhonemeToVisemeAssociations().cend()) { throw runtime_error("Double added phoneme association"); }
		if (Visemes().find(VisemeSymbol) == Visemes().cend()) { throw runtime_error("Unsupported viseme symbol"); }
		m_PhonemeToVisemeAssociations[PhonemeSymbol] = VisemeSymbol;
	}
}

void VisemeAnimatedMesh::InitialiseAnimationSettings(const nlohmann::json& settingsNode)
{
	if (settingsNode.contains("state_transition_time"))
	{
		m_StateTransitionTime = settingsNode["state_transition_time"];
	}
}

void VisemeAnimatedMesh::AddViseme(const std::string& symbol, const std::string& filePath)
{
	if (Visemes().find(symbol) != Visemes().end())
	{
		throw std::runtime_error("Added Viseme that's already present");
	}
	/* Structure: { "Vertices" : [ { "x": 0.1, "y": 0.5}, { "x": 0.2, "y": 0.4}]}*/
	std::ifstream InputFile = std::ifstream(filePath);
	if (!InputFile.good()) { return; }
	Viseme ToAdd;
	const nlohmann::json Config = nlohmann::json::parse(InputFile);
	for (const nlohmann::json& Vertex : Config["Vertices"])
	{
		Vector2D VertexToAdd;
		VertexToAdd.X = Vertex["x"];
		VertexToAdd.Y = Vertex["y"];
		ToAdd.Vertices.push_back(VertexToAdd);
	}
	m_Visemes[symbol] = ToAdd;
}

const std::string& VisemeAnimatedMesh::VisemeRootDirectory() const
{
	return m_VisemeRootDirectory;
}

const std::map<std::string, Viseme>& VisemeAnimatedMesh::Visemes() const
{
	return m_Visemes;
}

const std::map<std::string, std::string>& VisemeAnimatedMesh::PhonemeToVisemeAssociations() const
{
	return m_PhonemeToVisemeAssociations;
}

float VisemeAnimatedMesh::StateTransitionTime() const
{
	return m_StateTransitionTime;
}