#include "SpriteRenderer2D.h"
#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/util/StringCast.h>
#include "Entity.h"
#include "GameWorld.h"

const Tga::Sprite2DInstanceData& SpriteRenderer2D::GetInstance() const
{
	return myInstance;
}

void SpriteRenderer2D::SetInstance(const Tga::Sprite2DInstanceData& aInstance)
{
	myInstance = aInstance;
}

void SpriteRenderer2D::Read(const nlohmann::json& someData)
{
	myShared.myTexture = Tga::Engine::GetInstance()->GetTextureManager().GetTexture(string_cast<std::wstring>(someData["Texture"].get<std::string>()).c_str());

	Tga::Vector2ui textureSize = myShared.myTexture->CalculateTextureSize();
	auto size = someData["Size"];
	myInstance.mySize.x = size[0];
	if (myInstance.mySize.x == -1.f)
	{
		myInstance.mySize.x = static_cast<float>(textureSize.x);
	}
	myInstance.mySize.y = size[1];
	if (myInstance.mySize.y == -1.f)
	{
		myInstance.mySize.y = static_cast<float>(textureSize.y);
	}

	auto& sizeMultiplier = someData["SizeMultiplier"];
	myInstance.mySizeMultiplier.x = sizeMultiplier[0];
	myInstance.mySizeMultiplier.y = sizeMultiplier[1];

	auto& pivot = someData["Pivot"];
	myInstance.myPivot.x = pivot[0];
	myInstance.myPivot.y = pivot[1];

	auto& rect = someData["TextureRect"];
	myInstance.myTextureRect.myStartX = rect[0];
	myInstance.myTextureRect.myStartY = rect[1];
	myInstance.myTextureRect.myEndX = rect[2];
	myInstance.myTextureRect.myEndY = rect[3];

	myInstance.myRotation = someData["Rotation"];

	auto& color = someData["Color"];
	myInstance.myColor.myR = color[0];
	myInstance.myColor.myG = color[1];
	myInstance.myColor.myB = color[2];
	myInstance.myColor.myA = color[3];

	myOrderInLayer = someData["OrderInLayer"];
}

void SpriteRenderer2D::Start()
{
	myTransform = GetEntity()->GetComponent<Transform>();
}

void SpriteRenderer2D::Render(Tga::GraphicsEngine&)
{
	RenderBuffer& renderBuffer = GameWorld::GetInstance()->GetRenderBuffer();
	std::shared_ptr<Transform> transform = myTransform.lock();

	Tga::Sprite2DInstanceData instance = myInstance;
	instance.myRotation = myInstance.myRotation + transform->GetWorldEulerAngles().z;

	cu::Vector3<float> position = transform->GetWorldPosition();
	instance.myPosition = myInstance.myPosition;
	instance.myPosition.x += position.x;
	instance.myPosition.y += position.y;

	cu::Matrix4x4<float> scale = transform->GetScale();
	instance.mySizeMultiplier.x *= scale(1, 1);
	instance.mySizeMultiplier.y *= scale(2, 2);

	renderBuffer.Push(myShared, instance, myOrderInLayer);
}
