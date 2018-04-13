#include "stdafx.h"
#include "FBXLoader.h"

FBXLoader::FBXLoader()
{
}

FBXLoader::~FBXLoader()
{
}

VOID FBXLoader::ParseSkeletonHierarchy(FbxNode * pNode)
{
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		auto currNode = pNode->GetChild(i);
		ParseSkeletonHierarchyRecursively(currNode, 0, 0, -1);
	}
}

VOID FBXLoader::ParseSkeletonHierarchyRecursively(FbxNode * pNode, int inDepth, int myIndex, int inParentIndex)
{
	if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint jt;
		jt.mParentIndex = inParentIndex;
		jt.mName = pNode->GetName();
		mSkeleton.mVecJoint.push_back(jt);
	}
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ParseSkeletonHierarchyRecursively(pNode->GetChild(i), inDepth + 1, mSkeleton.mVecJoint.size(), myIndex);
	}
}

VOID FBXLoader::ParseNode(FbxNode * pNode)
{
	if (pNode->GetMesh())
	{
		ParseControlPoint(pNode);
		ParseVertexInfo(pNode);
		ParseMaterial(pNode);
	}
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ParseNode(pNode->GetChild(i));
	}
}

VOID FBXLoader::ParseControlPoint(FbxNode * pNode)
{
	auto pMesh = pNode->GetMesh();

	for (int i = 0; i < pMesh->GetControlPointsCount(); ++i)
	{
		VertexInfo pnt;
		pnt.mPos.x = static_cast<float>(pMesh->GetControlPointAt(i).mData[0]);
		pnt.mPos.y = static_cast<float>(pMesh->GetControlPointAt(i).mData[1]);
		pnt.mPos.z = static_cast<float>(pMesh->GetControlPointAt(i).mData[2]);

		ZeroMemory(pnt.mIndices, sizeof(pnt.mIndices));
		ZeroMemory(pnt.mWeight, sizeof(pnt.mWeight));
		mVecPNT.push_back(pnt);
	}
}

VOID FBXLoader::ParseVertexInfo(FbxNode * pNode)
{
	auto pMesh = pNode->GetMesh();
	auto SubsetNum = pMesh->GetElementMaterial()->GetIndexArray().GetLast() + 1;
	auto Subindex = new UINT[SubsetNum];
	UINT TotalIndex = 0;

	map<int, Subset> mapSubset;
	ZeroMemory(Subindex, sizeof(UINT) * SubsetNum);

	UINT SubsetIndexNum;
	for (int i = 0; i < pMesh->GetPolygonCount(); ++i)
	{
		SubsetIndexNum = pMesh->GetElementMaterial()->GetIndexArray().GetAt(i);
		UINT indexArr[3];
		for (int j = 0; j < pMesh->GetPolygonSize(i); ++j)
		{
			auto index = pMesh->GetPolygonVertex(i, j);
			ReadNormal(pMesh, index, TotalIndex, mVecPNT[index]);
			ReadUV(pMesh, index, pMesh->GetTextureUVIndex(i, j), mVecPNT[index]);
			ReadTangent(pMesh, index, TotalIndex, mVecPNT[index]);
			++TotalIndex;
			indexArr[j] = index;
		}
		mapSubset[SubsetIndexNum].mVecVertices.push_back(mVecPNT[indexArr[0]]);
		mapSubset[SubsetIndexNum].mVecVertices.push_back(mVecPNT[indexArr[2]]);
		mapSubset[SubsetIndexNum].mVecVertices.push_back(mVecPNT[indexArr[1]]);
		mapSubset[SubsetIndexNum].mVecIndices.push_back(Subindex[SubsetIndexNum]++);
		mapSubset[SubsetIndexNum].mVecIndices.push_back(Subindex[SubsetIndexNum]++);
		mapSubset[SubsetIndexNum].mVecIndices.push_back(Subindex[SubsetIndexNum]++);
	}
	for (int i = 0; i < mapSubset.size(); ++i)
	{
		mMapSubset[i] = mapSubset[i];
	}
	mVecPNT.clear();
}

VOID FBXLoader::ParseMaterial(FbxNode * pNode)
{
	auto materialCount = pNode->GetMaterialCount();
	for (int i = 0; i < materialCount; ++i)
	{
		auto surfaceMaterial = pNode->GetMaterial(i);
		ParseMaterialAttribute(surfaceMaterial, i, &mMapSubset[mMapSubset.size() - materialCount + i]);
		ParseMaterialTexture(surfaceMaterial, i, &mMapSubset[mMapSubset.size() - materialCount + i]);
	}
}

void FBXLoader::ParseMaterialAttribute(FbxSurfaceMaterial * inMaterial, UINT index, Subset* inObj)
{
	FbxDouble3 double3;

	Material matl;

	double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Ambient;
	matl.mAmbient.x = static_cast<float>(double3.mData[0]);
	matl.mAmbient.y = static_cast<float>(double3.mData[1]);
	matl.mAmbient.z = static_cast<float>(double3.mData[2]);
	matl.mAmbient.w = 1.0f;

	double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Diffuse;
	matl.mDiffuse.x = static_cast<float>(double3.mData[0]);
	matl.mDiffuse.y = static_cast<float>(double3.mData[1]);
	matl.mDiffuse.z = static_cast<float>(double3.mData[2]);
	matl.mDiffuse.w = 1.0f;

	double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Specular;
	matl.mSpecular.x = static_cast<float>(double3.mData[0]);
	matl.mSpecular.y = static_cast<float>(double3.mData[1]);
	matl.mSpecular.z = static_cast<float>(double3.mData[2]);
	matl.mSpecular.w = 1.0f;

	double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Reflection;
	matl.mReflect.x = static_cast<float>(double3.mData[0]);
	matl.mReflect.y = static_cast<float>(double3.mData[1]);
	matl.mReflect.z = static_cast<float>(double3.mData[2]);
	matl.mReflect.w = 1.0f;

	inObj->mMaterial = matl;
}

void FBXLoader::ParseMaterialTexture(FbxSurfaceMaterial * inMaterial, UINT index, Subset * inObj)
{
	int textureIndex = 0;
	FbxProperty property;

	FBXSDK_FOR_EACH_TEXTURE(textureIndex)
	{
		property = inMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
		if (property.IsValid())
		{
			UINT textureCount = property.GetSrcObjectCount<FbxTexture>();
			for (UINT i = 0; i < textureCount; i++)
			{
				FbxLayeredTexture* pLayeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
				if (!pLayeredTexture)
				{
					FbxTexture* texture = property.GetSrcObject<FbxTexture>(i);
					if (texture)
					{
						string textureType = property.GetNameAsCStr();
						FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);

						if (fileTexture)
						{
							string name = fileTexture->GetFileName();
							int index = 0;
							index = name.find_last_of('/');

							if (index == -1)
								index = name.find_last_of('\\');

							if (index != -1)
								name = name.substr(index + 1, strlen(name.c_str()));

							//name = sPath + name;
							if (textureType == "DiffuseColor")
							{
								inObj->mDiffuseTexture = fileTexture->GetFileName();
							}
							else if (textureType == "SpecularColor")
							{
								inObj->mSpecularTexture = fileTexture->GetFileName();
							}
							else if (textureType == "NormalMap")
							{
								inObj->mNormalTexture = fileTexture->GetFileName();
							}
						}
					}
				}
			}
		}
	}
}

VOID FBXLoader::ReadNormal(FbxMesh * pMesh, UINT mPolygonIndex, UINT IndexNum, VertexInfo & pnt)
{
	FbxGeometryElementNormal* pVertexNormal = pMesh->GetElementNormal(0);
	switch (pVertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (pVertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pnt.mNormal.x = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(mPolygonIndex).mData[0]);
			pnt.mNormal.y = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(mPolygonIndex).mData[1]);
			pnt.mNormal.z = -static_cast<float>(pVertexNormal->GetDirectArray().GetAt(mPolygonIndex).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = pVertexNormal->GetIndexArray().GetAt(mPolygonIndex);
			pnt.mNormal.x = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[0]);
			pnt.mNormal.y = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[1]);
			pnt.mNormal.z = -static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (pVertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pnt.mNormal.x = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(IndexNum).mData[0]);
			pnt.mNormal.y = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(IndexNum).mData[1]);
			pnt.mNormal.z = -static_cast<float>(pVertexNormal->GetDirectArray().GetAt(IndexNum).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = pVertexNormal->GetIndexArray().GetAt(IndexNum);
			pnt.mNormal.x = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[0]);
			pnt.mNormal.y = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[1]);
			pnt.mNormal.z = -static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
		break;
	}
}

VOID FBXLoader::ReadUV(FbxMesh * pMesh, UINT mPolygonIndex, UINT UVIndex, VertexInfo & pnt)
{
	FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(0);

	switch (pVertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (pVertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pnt.mUV.x = static_cast<float>(pVertexUV->GetDirectArray().GetAt(mPolygonIndex).mData[0]);
			pnt.mUV.y = 1.0f - static_cast<float>(pVertexUV->GetDirectArray().GetAt(mPolygonIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = pVertexUV->GetIndexArray().GetAt(mPolygonIndex);
			pnt.mUV.x = static_cast<float>(pVertexUV->GetDirectArray().GetAt(index).mData[0]);
			pnt.mUV.y = 1.0f - static_cast<float>(pVertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (pVertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			pnt.mUV.x = static_cast<float>(pVertexUV->GetDirectArray().GetAt(UVIndex).mData[0]);
			pnt.mUV.y = 1.0f - static_cast<float>(pVertexUV->GetDirectArray().GetAt(UVIndex).mData[1]);
		}
		break;
		}
	}
}

VOID FBXLoader::ReadTangent(FbxMesh * pMesh, UINT mPolygonIndex, UINT IndexNum, VertexInfo & pnt)
{
	FbxGeometryElementTangent* vertexTangent = pMesh->GetElementTangent(0);
	if (!vertexTangent) return;

	switch (vertexTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pnt.mTangentU.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(mPolygonIndex).mData[0]);
			pnt.mTangentU.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(mPolygonIndex).mData[1]);
			pnt.mTangentU.z = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(mPolygonIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(mPolygonIndex);
			pnt.mTangentU.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			pnt.mTangentU.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			pnt.mTangentU.z = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pnt.mTangentU.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(IndexNum).mData[0]);
			pnt.mTangentU.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(IndexNum).mData[1]);
			pnt.mTangentU.z = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(IndexNum).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(IndexNum);
			pnt.mTangentU.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			pnt.mTangentU.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			pnt.mTangentU.z = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
}
FbxManager* FBXLoader::mFbxSdkManager = nullptr;

BOOL FBXLoader::LoadFBX(const string fileName)
{
	FILE* fp;

	fopen_s(&fp, fileName.c_str(), "r");

	mFbxSdkManager = nullptr;
	if (!mFbxSdkManager)
	{
		mFbxSdkManager = FbxManager::Create();
		FbxIOSettings* pIOsettings = FbxIOSettings::Create(mFbxSdkManager, IOSROOT);
		mFbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImport = FbxImporter::Create(mFbxSdkManager, "");
	FbxScene *fbxScene = FbxScene::Create(mFbxSdkManager, "");

	bool bSuccess = pImport->Initialize(fileName.c_str(), -1, mFbxSdkManager->GetIOSettings());
	if (!bSuccess)
		return false;

	bSuccess = pImport->Import(fbxScene);
	if (!bSuccess)
		return false;

	pImport->Destroy();

	printf("%s을 불러오고 있습니다\n", fileName.c_str());
	ParseSkeletonHierarchy(fbxScene->GetRootNode());

	if (mSkeleton.mVecJoint.empty())
	{
		printf("no skeleton \n");
		IsBoneAnimation = false;
	}
	else
		IsBoneAnimation = true;

	ParseNode(fbxScene->GetRootNode());
	fclose(fp);

	return true;
}