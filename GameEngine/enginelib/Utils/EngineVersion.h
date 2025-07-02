#ifndef ENGINEVERSION_H
#define ENGINEVERSION_H

#include "VersionEngine.h"

#include <string>

namespace fts
{
    struct VersionInfo
    {

    public:
        /// Initialize with the specified version number
        inline VersionInfo(int major, int minor, int patch, std::string extra)
            : major_(major)
            , minor_(minor)
            , patch_(patch)
            , extra_(extra)
        {
        }

        /**
         * \brief Parse a version string of the form "major.minor.patch.extra"
         * and turn it into a \ref Version structure
         */
        VersionInfo(const std::string& versionString);

        VersionInfo();

        /// Check if this program version is \a older than \c other
        inline bool operator<(const VersionInfo& other) const
        {
            if(major_ < other.major_)
                return true;
            else if(major_ > other.major_)
                return false;
            else if(minor_ < other.minor_)
                return true;
            else if(minor_ > other.minor_)
                return false;
            else if(patch_ < other.patch_)
                return true;
            else
                return false;
        }

        /// Check if this program version is \a older than or equal to \c other
        inline bool operator<=(const VersionInfo& other) const
        {
            return *this < other || *this == other;
        }

        /// Check if two program versions match
        inline bool operator==(const VersionInfo& other) const
        {
            return major_ == other.major_
                   && minor_ == other.minor_
                   && patch_ == other.patch_;
        }

        /// Is this a valid version number?
        inline bool isValid()
        {
            return major_ != 0 || minor_ != 0 || patch_ != 0;
        }

        /// Are the following two versions compatible?
        inline bool isCompatible(const VersionInfo& other) const
        {
            return major_ == other.major_ && minor_ == other.minor_;
        }

        std::string toString() const;
        std::string toStringComplete() const;

        static std::string getRootResourcePath();

        inline int getMajorVersion() const
        {
            return major_;
        }
        inline int getMinorVersion() const
        {
            return minor_;
        }
        inline int getPatch() const
        {
            return patch_;
        }
        inline std::string getExtra() const
        {
            return extra_;
        }

        static std::string getPathRelativeRoot(const std::string& path);
        static std::string getPathRelativeBinary(const std::string& path);

    private:
        int major_;
        int minor_;
        int patch_;
        std::string extra_;
    };
} // namespace fts

#endif // ENGINEVERSION_H
