#pragma once
#include <vector>
#include "EntityTemplate.h"
#include "EditorEntity.h"

class NodeCollection
{
public:
	NodeCollection();
	virtual ~NodeCollection() = default;

	typedef std::vector<std::shared_ptr<NodeCollection>>::iterator iterator;
	typedef std::vector<std::shared_ptr<NodeCollection>>::const_iterator const_iterator;

	void SetName(const std::string& aName);
	const std::string& GetName() const;


	virtual bool BeginDisplay();
	void DisplayChildren(std::vector<std::shared_ptr<NodeCollection>>& aOutClicked, bool aAllowDrop, bool* aOutDirty = nullptr);
	virtual void EndDisplay();

	void AddChild(std::shared_ptr<NodeCollection> aChild);

	void SetDisabled(bool aDisabled);
	bool IsDisabled() const;

	void SetClicked();
	bool EatClicked();

#pragma region Collection

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	void clear();
	size_t size() const;

#pragma endregion

	virtual bool ShouldBeRemoved() = 0;

protected:
	virtual void OnChildAdded(std::shared_ptr<NodeCollection> aNode) {};

private:
	void SetParent(NodeCollection* aParent);
	static void Move(NodeCollection* aFromParent, const_iterator aFrom, NodeCollection* aToParent, const_iterator aTo);

	struct DragDropPayload
	{
		NodeCollection* CurrentParent;
		const_iterator Iterator;
	};

	void FilterChildren();

	std::string myName;
	std::vector<std::shared_ptr<NodeCollection>> myChildren;

	float myContentsSizeY = 1.f;
	bool myIsDisabled;
	bool myIsClicked;
	NodeCollection* myParent;
};

class EditorTree : public NodeCollection
{
public:
	class Node : public NodeCollection
	{
	public:
		Node(bool aDisabled, EntityTemplate aTemplate);
		~Node();

		void SetTemplate(const EntityTemplate& aTemplate);
		EntityTemplate GetTemplate();
		void MakeIndependent();
		void MakePrefab(const std::string& aPath);

		void ImGui_ObjectEditor();

		std::shared_ptr<EditorEntity> GetEditorEntity();

		void Destroy();
		bool IsDestroyed() const;

		void SetPosition(const cu::Vector3<float>& aPosition);
		void PropegateShowing(bool aShowing);

		virtual bool BeginDisplay() override;

		virtual bool ShouldBeRemoved() { return IsDestroyed(); };

	protected:
		virtual void OnChildAdded(std::shared_ptr<NodeCollection> aChild) override;

	private:
		void OverwritePrefab();
		void MakeIntoPrefab();

		bool CheckSavePrefab(const std::string& aPath) const;
		void ImGui_ShowPrefabNameInputPopup();
		//EntityTemplate myTemplate;
		std::shared_ptr<EditorEntity> myEditorEntity;

		//bool myTemplateIsOutdated;
	};

	//void Display();
	virtual bool ShouldBeRemoved() override { return false; };

protected:
	virtual void OnChildAdded(std::shared_ptr<NodeCollection> aChild) override;

private:
	//void DisplayNode(std::shared_ptr<Node> aNode, bool aCanEdit);

	//std::vector<std::shared_ptr<Node>> myRootNodes;
};