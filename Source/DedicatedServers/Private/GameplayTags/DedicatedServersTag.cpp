#include "GameplayTags/DedicatedServersTag.h"


#define GAME_SESSIONS_API_TAG(name) "DedicatedServersTag.GameSessionsAPI." name

namespace DedicatedServersTag::GameSessionsAPI
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleets, GAME_SESSIONS_API_TAG("ListFleets"),
		"List Fleets resource on the GameSessions API");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSession, GAME_SESSIONS_API_TAG("FindOrCreateGameSession"),
		"Find or Create Game Session resource on the GameSessions API");
}
