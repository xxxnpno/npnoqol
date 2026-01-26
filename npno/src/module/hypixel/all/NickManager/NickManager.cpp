#include "NickManager.h"

hypixel::NickManager::NickManager()
    : HypixelStatsModule{ true, HypixelGamemode::Gamemode::ALL }
{
    this->nickHashes = 
    {
        "4c7b0468044bfecacc43d00a3a69335a834b73937688292c20d3988cae58248d",
        "3b60a1f6d562f52aaebbf1434f1de147933a3affe0e764fa49ea057536623cd3",
        "19875bb4ac8e7e68c122fdf22bf99abeb4326b96c58ec21d4c5b64cc7a12a5",
        "dd2f967eee43908cda7854df9eb7263637573fd10e498dcdf5d60e9ebc80a1e5",
        "21c44f6b47eadd6720ddc1a14dc4502bd6ccee6542efb74e2b07adb65479cc5",
        "7162726e3b3a7f9c515749a18723ee4439dadd26c0d60e07dea0f2267c6f40a7",
        "10e62bc629872c7d91c2f2edb9643b7455e7238a8c9b4074f1c5312ef162ba22",
        "4336ff82b3d2d7b9081fec5adec2943329531c605b657c11b35231c13a0b8571",
        "173ec57a878e2b5b0922e34be6acac108372f34dace9871a894fe15ed8",
        "7f73526b1a9379be41301cfb74c55270186fbaca63df6949ce3d626e79304d92",
        "7d91aee3b51f3f8d92df52575e5755d97977dcdfb38e74488c613411829e32",
        "8e42e588e1d09ce03c79463e94a7664304f688caf4c617dbcbca64a635bbe79",
        "8f1f9b3919c879f4ec251871c19b20725bc76d657762b5ddfdf3a5ff4f82cb47",
        "989bc66d66ff0513507bcb7aa27f4e7e2af24337c4e7c4786c4630839966fdf7",
        "bdfc818d40b635bcd3d2e3d3b977651d0da0eea87f13847b99dc7bea3338b",
        "5841684ec6a1f8ba919046857dac9175514fef18a2c9395dc3e341b9f5e778ac",
        "211e121448c83125c945cd238dc4f4c5e9a320df5ee3c8c9ad3bb80c055db876",
        "3cce5c4d27979e98afe0d43acca8ebddc7e74a4e62480486e62ee3512",
        "68d98f9a56d4c0ab805c6805756171f4a2cdbf5fa8ce052a4bf4f86411fb080",
        "e2629467cf544e79ed148d3d3842876f2d8841898b4b2e6f60dfc1e02f1179f3",
        "6162abdfb5c7ace7d2caaabdc5d4fdfc32fb63f2a56db69f276167dffce41",
        "af336a55d17916836ce0ed102cbdb0fa6376544971301e0f28beb3899c649ff2",
        "1580729b12e1d6357e7eaa088fbf06ba2c8a2fb35b0ba4c5451937d3f134ca9",
        "1f72e604cdb4c49f7106b594ac87eff3ed6a1999255437241f57d28c45d103f",
        "542a699fe314b9f747eed89b9cae23fdefc27684f6c13dc4a29f5d057cc12d",
        "b2a4cd136505395a876113a800aa8ec15761ca3d945b57e0d0dcdcfeafd7a6d9",
        "907fcce8c3a8d337a6aff226847e3cc7c9bb6bd02f43be1b7c71b3dcd244e11",
        "62a6c3e6b8cbd4fbcb5866289642bb5b5a90dd16e2c28dc054c9d248943834f9",
        "173481eb7f2157a5ad79ec765d36be5736395b72ee519185e23f436ba15185",
        "9ad4ffb57819e7ff633f60b5901e44a3570573ad4d453075b72ae2cbd23c8a6d"
    };
}

hypixel::NickManager::~NickManager() = default;

auto hypixel::NickManager::Update() -> void
{
    this->SkinDenicker();

    for (auto& [name, player] : HypixelAPI::GetNickList())
    {
        if (!player.warned)
        {
            player.warned = true;
            this->Warn(name, player.realName);
        }
    }
}

auto hypixel::NickManager::SkinDenicker() -> void
{
    for (const std::unique_ptr<EntityPlayer>& player : mc->GetTheWorld()->GetPlayerEntities())
    {
        if (this->IsBot(player))
        {
            continue;
        }

        if (this->parsedPlayers.find(player->GetName()) != this->parsedPlayers.end())
        {
             continue;
        }

        this->parsedPlayers.insert(player->GetName());

        const std::unique_ptr<GameProfile>& profile = player->GetGameProfile();

        for (const std::unique_ptr<Property>& property : profile->GetProperties()->GetAllProperties())
        {
            if (property->GetName() == "textures")
            {
                const std::string encoded = JavaUtil::JStringToString(property->GetValue());
                const std::string decoded = Base64::Decode(encoded);

                try
                {
                    auto json = nlohmann::json::parse(decoded);
                    std::string skinUrl = json.at("textures").at("SKIN").at("url");
                    this->ParseSkinData(player->GetName(), skinUrl, profile->GetName());
                }
                catch (...)
                {
                    continue;
                }

                break;
            }
        }
    }
}

auto hypixel::NickManager::ParseSkinData(const std::string& playerName, const std::string& skinHash, const std::string& profileName) -> void
{
    if (!this->IsNickHash(skinHash) and !profileName.empty() and profileName != playerName)
    {
        std::map<std::string, HypixelAPI::Nick>& nickList = HypixelAPI::GetNickList();
        if (nickList.find(playerName) != nickList.end())
        {
            nickList[playerName].realName = profileName;
        }
    }
}

auto hypixel::NickManager::IsNickHash(const std::string& hash) -> bool
{
    return this->nickHashes.find(hash) != this->nickHashes.end();
}

auto hypixel::NickManager::Warn(const std::string& name, const std::string& realName) -> void
{
    std::string message;
    if (!realName.empty())
    {
        message = std::format("{}{} {}is nicked as {}{}", 
            MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_AQUA),
            realName,
            MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA),
            MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_AQUA),
            name);
    }
    else
    {
        message = std::format("{}{} {}is nicked",
            MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_AQUA),
            name,
            MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA));
    }

    mc->GetThePlayer()->AddChatMessage(std::make_unique<ChatComponentText>(message));
}