#include "pkUUID.h"
#include "uuid.h"

namespace pkEngineSDK
{
const String UUID::PK_NAME_GEN_SEED = "47183823-2574-4bfd-b411-99ed177d3e43";
uuids::uuid_name_generator gen(uuids::uuid::from_string(UUID::PK_NAME_GEN_SEED).value());

String
UUID::generateRandomUUID()
{
  std::mt19937 engine{ std::random_device{}() };
  uuids::uuid_random_generator gen{ engine };
  uuids::uuid id = gen();
  return uuids::to_string(id);
}

String
UUID::generateRandomUUIDFromString(const String _string)
{
  uuids::uuid const id = gen(_string);
  return uuids::to_string(id);
}
}