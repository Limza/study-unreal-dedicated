#include "GameplayTags/DedicatedServersTag.h"


#define GAME_SESSIONS_API_TAG(name) "DedicatedServersTag.GameSessionsAPI." name

namespace DedicatedServersTag::GameSessionsApi
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleets, GAME_SESSIONS_API_TAG("ListFleets"),
		"List Fleets resource on the GameSessions API");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSession, GAME_SESSIONS_API_TAG("FindOrCreateGameSession"),
		"Find or Create Game Session resource on the GameSessions API");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CreatePlayerSession, GAME_SESSIONS_API_TAG("CreatePlayerSession"),
		"Creates a new Player Session on the GameSessions API");
}
