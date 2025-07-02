#include "FilePath.h"

#include <Utils/EngineVersion.h>
#include <ftspch.h>

namespace fts
{
    VersionInfo::VersionInfo(const std::string& versionString)
    {
        std::istringstream iss(versionString);
        std::string str[4];

        for(int i = 0; i < 4; ++i)
            std::getline(iss, str[i], '.');

        std::istringstream(str[0]) >> major_;
        std::istringstream(str[1]) >> minor_;
        std::istringstream(str[2]) >> patch_;
        std::istringstream(str[3]) >> extra_;
    }

    VersionInfo::VersionInfo()
    {
        std::istringstream iss(GAME_ENGINE_VERSION);
        std::string str[4];

        for(int i = 0; i < 4; ++i)
            std::getline(iss, str[i], '.');

        std::istringstream(str[0]) >> major_;
        std::istringstream(str[1]) >> minor_;
        std::istringstream(str[2]) >> patch_;
        std::istringstream(str[3]) >> extra_;
    }

    std::string VersionInfo::toString() const
    {
        std::stringstream ss;
        ss << GAME_ENGINE_VERSION;
        return ss.str();
    }

    std::string VersionInfo::toStringComplete() const
    {
        std::stringstream ss;
        ss << toString() << " (" << GAME_ENGINE_VERSION;
        ss << ", 64 Bit";
        ss << ")";
        return ss.str();
    }

    std::string VersionInfo::getRootResourcePath()
    {
        std::stringstream ss;
        ss << GAME_ENGINE_ROOT_RESOURCE;
        return ss.str();
    }

    std::string VersionInfo::getPathRelativeRoot(const std::string& path)
    {
        util::FilePath pathRoot(getRootResourcePath() + std::string("/") + path);
        return pathRoot;
    }

    std::string VersionInfo::getPathRelativeBinary(const std::string& path)
    {
        util::FilePath pathRoot("../../../" + path);
        return pathRoot;
    }
} // namespace fts
