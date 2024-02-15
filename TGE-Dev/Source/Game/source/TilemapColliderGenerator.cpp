#include "TilemapColliderGenerator.h"
#include "Tilemap.h"
#include "Entity.h"
#include "EntityTemplate.h"
#include "GameWorld.h"
#include "RectangleCollider.h"

void TilemapColliderGenerator::Start()
{
	auto gameWorld = GameWorld::GetInstance();

	std::shared_ptr<Tilemap> tilemap = GetEntity()->GetComponent<Tilemap>();
	std::shared_ptr<Transform> transform = GetEntity()->GetComponent<Transform>();

	const std::vector<Tilemap::Tile>& tiles = tilemap->GetTiles();

	int minX = INT_MAX, maxX = INT_MIN;
	int minY = INT_MAX, maxY = INT_MIN;

	std::set<std::pair<int, int>> tileSet;
	for (const Tilemap::Tile& tile : tiles)
	{
		tileSet.emplace(std::pair<int, int>{ tile.Position.x, tile.Position.y });
		if (tile.Position.x < minX) minX = tile.Position.x;
		if (tile.Position.x > maxX) maxX = tile.Position.x;
		if (tile.Position.y < minY) minY = tile.Position.y;
		if (tile.Position.y > maxY) maxY = tile.Position.y;
	}

	std::vector<std::vector<cu::AABB2D<int>>> candidates;

	for (int x = minX; x <= maxX; x++)
	{
		candidates.push_back({});
		cu::AABB2D<int> candidate;
		candidate.Min.x = x;
		candidate.Max.x = x + 1;
		bool counting = false;
		for (int y = minY; y <= maxY; y++)
		{
			auto it = tileSet.find(std::pair(x, y));
			if (it != tileSet.end())
			{
				tileSet.erase(it);
				if (!counting)
				{
					candidate.Min.y = y;
					counting = true;
				}
			}
			else if (counting)
			{
				counting = false;
				candidate.Max.y = y;
				candidates[x - minX].push_back(candidate);
			}
		}
		if (counting)
		{
			counting = false;
			candidate.Max.y = maxY + 1;
			candidates[x - minX].push_back(candidate);
		}
	}

	for (size_t x = 1; x < candidates.size(); x++)
	{
		auto it = candidates[x].begin();
		while (it < candidates[x].end())
		{
			auto prevIt = candidates[x - 1].begin();
			while (prevIt < candidates[x - 1].end())
			{
				if (prevIt->Min.y == it->Min.y && prevIt->Max.y == it->Max.y)
				{
					prevIt->Max.x = it->Max.x;
					*it = *prevIt;
					prevIt = candidates[x - 1].erase(prevIt);
					continue;
				}
				++prevIt;
			}
			++it;
		}
	}

	EntityTemplate2 tileCollider;
	tileCollider.AddComponent("Transform");
	tileCollider.AddComponent("RectangleCollider");

	for (const std::vector<cu::AABB2D<int>>& candidateCollection : candidates)
	{
		for (const cu::AABB2D<int>& bounds : candidateCollection)
		{
			std::shared_ptr<Entity> entity = gameWorld->GetEntityManager().Add(tileCollider, gameWorld->GetComponentRegistry());
			std::shared_ptr<RectangleCollider> collider = entity->GetComponent<RectangleCollider>();
			std::shared_ptr<Transform> otherTransform = entity->GetComponent<Transform>();

			otherTransform->SetParent(transform.get(), false);
			collider->SetBounds(cu::AABB2D<float>({ static_cast<float>(bounds.Min.x) * 32.f, static_cast<float>(bounds.Min.y) * 32.f }, { static_cast<float>(bounds.Max.x) * 32.f, static_cast<float>(bounds.Max.y) * 32.f }));
			collider->SetStatic(true);
		}
	}
}
