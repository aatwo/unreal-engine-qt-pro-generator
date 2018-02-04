#ifndef GLOBAL_H
#define GLOBAL_H


#include <QString>


struct Global
{
    void clearProjectPaths()
    {
        unrealProjectDir.clear();
        unrealProjectFile.clear();
        projectName.clear();
        projectFileDir.clear();
        visualStudioProjectFile.clear();
        qtProjectFile.clear();
        qtDefinesFile.clear();
        qtIncludesFile.clear();
    }

    void clearUnrealEnginePaths()
    {
        unrealEngineBatDir.clear();
        unrealEngineBuildBatFile.clear();
        unrealEngineCleanBatFile.clear();
        unrealEngineBinaryDir.clear();
        unrealEngineEditorFile.clear();
    }

    void clearAll()
    {
        clearProjectPaths();
        clearUnrealEnginePaths();
    }

    // Project paths
    QString unrealBuildDir;
    QString unrealBinaryDir;
    QString unrealProjectDir;
    QString unrealProjectFile;
    QString projectName;
    QString projectFileDir;
    QString visualStudioProjectFile;
    QString qtProjectFile;
    QString qtDefinesFile;
    QString qtIncludesFile;

    // Unreal engine paths
    QString unrealEngineBatDir;
    QString unrealEngineBuildBatFile;
    QString unrealEngineCleanBatFile;
    QString unrealEngineBinaryDir;    // C:/Program Files/Epic Games/UE_4.17/Engine/Binaries/Win64
    QString unrealEngineEditorFile;   // C:/Program Files/Epic Games/UE_4.17/Engine/Binaries/Win64/UE4Editor.exe


}; // Global


#endif // GLOBAL_H
