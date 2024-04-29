#include <fbxsdk.h>

int main() {
    // Initialize FBX SDK
    FbxManager* sdkManager = FbxManager::Create();
    FbxScene* scene = FbxScene::Create(sdkManager, "SceneName");

    FbxImporter* importer = FbxImporter::Create(sdkManager, "");
    bool importSuccess = importer->Initialize("humanoid.fbx", -1, sdkManager->GetIOSettings());
    if (importSuccess) {
        importer->Import(scene);
        importer->Destroy();
    }
    else {
        // Error handling
        importer->Destroy();
        return 1;
    }

    // Clean up
    scene->Destroy();
    sdkManager->Destroy();

    return 0;
}