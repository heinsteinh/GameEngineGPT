#ifndef RESOURCEPATH_H
#define RESOURCEPATH_H

#include <string>

namespace fts
{
    std::string getRootResourcePath();

    std::string getGameExampleRootPath();
    std::string getGameCmdRootPath();

    std::string getPathRelativeRoot(const std::string& path);

    std::string getPathGameExample(const std::string& path);
    std::string getPathGameCmd(const std::string& path);

    std::string getPathRelativeBinary(const std::string& path);
} // namespace fts

#endif // RESOURCEPATH_H
