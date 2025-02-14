// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
#pragma once
#include "Microsoft/SQLiteIndex.h"
#include "Public/AppInstallerRepositorySource.h"
#include <AppInstallerSynchronization.h>

#include <memory>


namespace AppInstaller::Repository::Microsoft
{
    // A source that holds a SQLiteIndex and lock.
    struct SQLiteIndexSource : public std::enable_shared_from_this<SQLiteIndexSource>, public ISource
    {
        SQLiteIndexSource(const SourceDetails& details, std::string identifier, SQLiteIndex&& index, Synchronization::CrossProcessReaderWriteLock&& lock = {}, bool isInstalledSource = false);

        SQLiteIndexSource(const SQLiteIndexSource&) = delete;
        SQLiteIndexSource& operator=(const SQLiteIndexSource&) = delete;

        SQLiteIndexSource(SQLiteIndexSource&&) = default;
        SQLiteIndexSource& operator=(SQLiteIndexSource&&) = default;

        ~SQLiteIndexSource() = default;

        // Get the source's details.
        const SourceDetails& GetDetails() const override;

        // Gets the source's identifier; a unique identifier independent of the name
        // that will not change between a remove/add or between additional adds.
        // Must be suitable for filesystem names.
        const std::string& GetIdentifier() const override;

        // Execute a search on the source.
        SearchResult Search(const SearchRequest& request) const override;

        // Gets the index.
        const SQLiteIndex& GetIndex() const { return m_index; }

        // Determines if the other source refers to the same as this.
        bool IsSame(const SQLiteIndexSource* other) const;

    private:
        SourceDetails m_details;
        Synchronization::CrossProcessReaderWriteLock m_lock;
        bool m_isInstalled;
    protected:
        SQLiteIndex m_index;
    };

    // A source that holds a SQLiteIndex and lock.
    struct SQLiteIndexWriteableSource : public SQLiteIndexSource, public IMutablePackageSource
    {
        SQLiteIndexWriteableSource(const SourceDetails& details, std::string identifier, SQLiteIndex&& index, Synchronization::CrossProcessReaderWriteLock&& lock = {}, bool isInstalledSource = false);

        // Adds a package version to the source.
        void AddPackageVersion(const Manifest::Manifest& manifest, const std::filesystem::path& relativePath);

        // Removes a package version from the source.
        void RemovePackageVersion(const Manifest::Manifest& manifest, const std::filesystem::path& relativePath);
    };
}
