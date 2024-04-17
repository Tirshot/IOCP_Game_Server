#include "pch.h"
#include "Inventory.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "FileManager.h"
#include "ChatManager.h"
#include "InputManager.h"
#include "TextBox.h"

Inventory::~Inventory()
{
    
}

void Inventory::BeginPlay()
{
    _itemTable = GET_SINGLE(FileManager)->GetDataFromCSV("E:\\Cpp\\IOCP\\Server\\Client\\Resources\\Table\\ItemTable.csv");
    _background = GET_SINGLE(ResourceManager)->GetSprite(L"Inventory");
    _itemSprite = GET_SINGLE(ResourceManager)->GetSprite(L"Sword");
    _invenRect = { 485, 160, 770, 460 };
    _slots.assign(40, { 0 });

    { // ������ �̸�
        wstring wstr = L"������ �̸�";
        _itemName = new TextBox(wstr);
        _itemName->SetPos({ 605, 350 });
        _itemName->SetSize({ 155, 25 });
        _itemName->SetPadding(5, 5);
        AddChild(_itemName);
    }
    { // ������ ����
        wstring wstr = L"����";
        _itemCount = new TextBox(wstr);
        _itemCount->SetPos({ 710, 350 });
        _itemCount->SetSize({ 40, 25 });
        _itemCount->SetPadding(5, 5);
        _itemCount->SetVisible(false);
        AddChild(_itemCount);
    }
    { // ������ ����
        wstring wstr = L"������ ����";
        _itemDescription = new TextBox(wstr);
        _itemDescription->SetPos({ 605, 375 });
        _itemDescription->SetSize({ 155, 75 });
        _itemDescription->SetPadding(5, 5);
        AddChild(_itemDescription);
    }
    
    // �� ���Կ� RECT ����
    SetSlotRects();

    for (int i = 0; i < 5; i++)
        AddItem(i);

}

void Inventory::Tick()
{
    _mousePos = GET_SINGLE(InputManager)->GetMousePos();

    // ������ ���� �ʱ�ȭ
    for (int i = 0; i < _slots.size(); i++)
    {
        // ������ �������� ���Կ� �Ҵ�
        SetItemSlot(_slots[i]);

        // �� ���Կ� RECT �Ҵ�
        _slots[i].Rect = _rects[i];
    }

    // ������ �巡�� �� ���
    for (auto& slot : _slots)
    {
        if (IsMouseInRect(slot.Rect))
        {
            // ������ ����
            _itemName->SetText(slot.KorName);
            _itemDescription->SetText(slot.Description);

            // �巡�� ����
            if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
            {
                // �� ������ �巡�� �Ұ�
                if (slot.ItemId <= 0)
                    continue;

                _selectedItem = &slot;
            }
            else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
            {
                // �ٸ� �������� ���
                if (_selectedItem != nullptr)
                {
                    _destinatedItem = &slot;
                    ChangeItem(*_selectedItem, *_destinatedItem);
                }
                _selectedItem = nullptr;
            }

            // ������ ���� ǥ��
            if (slot.Type == L"Consumable")
            {
                _itemCount->SetVisible(true);
                _itemCount->SetText(to_wstring(slot.ItemCount) + L"��");
            }
            else
            {
                _itemCount->SetVisible(false);
            }
        }
        // �κ��丮 �ٱ����� ���
        if (GET_SINGLE(InputManager)->IsMouseOutRect(_invenRect))
        {
            if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
            {
                if (_selectedItem != nullptr)
                {
                    RemoveItem(*_selectedItem);
                    _selectedItem = nullptr;
                }
            }
        }
    }
}

void Inventory::Render(HDC hdc)
{
    int _initialX = _pos.x + 6;
    int _initialY = _pos.y + 42;

    ::TransparentBlt(hdc,
        _pos.x ,
        _pos.y,
        _background->GetSize().x,
        _background->GetSize().y,
        _background->GetDC(),
        0,
        0,
        _background->GetSize().x,
        _background->GetSize().y,
        _background->GetTransparent());

    for (int i = 0; i < 40; i++)
    {
        if (_slots[i].Sprite == nullptr)
            continue;

        float spritePosX = _slots[i].Rect.left;
        float spritePosY = _slots[i].Rect.top - 2;

        ::TransparentBlt(hdc,
            spritePosX,
            spritePosY,
            _slotSize,
            _slotSize,
            _slots[i].Sprite->GetDC(),
            0,
            0,
            _slots[i].Sprite->GetSize().x,
            _slots[i].Sprite->GetSize().y,
            _slots[i].Sprite->GetTransparent());

        if (_slots[i].ItemId == 0)
            break;
    }

    for (auto& child : _children)
        if (child->GetVisible())
            child->Render(hdc);

    if (_selectedItem == nullptr)
    {
        return;
    }
    else
    {
        ::TransparentBlt(hdc,
            _mousePos.x,
            _mousePos.y,
            _slotSize,
            _slotSize,
            _selectedItem->Sprite->GetDC(),
            0,
            0,
            _selectedItem->Sprite->GetSize().x,
            _selectedItem->Sprite->GetSize().y,
            _selectedItem->Sprite->GetTransparent());
    }
}

void Inventory::SetItemSlot(ITEM& slot)
{
    if (slot.ItemId <= 0)
        return;

    // �̸��� Sprite, Rect, Description, Type �Ҵ�
    vector<wstring> ItemInfo = findItemInfo(slot.ItemId);

    slot.Name = GetName(ItemInfo);
    slot.KorName = GetKorName(ItemInfo);
    slot.Description = GetDescription(ItemInfo);
    slot.Type = GetType(ItemInfo);
    slot.Sprite = GetSprite(slot.Name);
}

void Inventory::SetSlotRects()
{
    int _initialX = _pos.x + 13;
    int _initialY = _pos.y + 45;

    // �� ���Կ� Rect �Ҵ�
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            RECT Rect = { _initialX + (34 * i), _initialY + (34 * j), _initialX + (34 * i) + 22,  _initialY + (34 * j) + _slotSize };
            _rects.push_back(Rect);
        }
    }
}

bool Inventory::AddItem(int ItemId)
{
    bool found = false;

    // _slots ��ȸ
    for (auto& slot : _slots)
    {
        // �������� �̹� �����ϴ� ���
        if (slot.ItemId == ItemId)
        {
            slot.ItemCount++; // ���� ����
            found = true;
            return true;
        }
    }

    // �������� �������� �ʴ� ���
    if (found == false)
    {
        for (auto& slot : _slots)
        {
            if (slot.ItemId == 0)
            {
                auto ItemInfo = findItemInfo(ItemId);
                slot.ItemId = ItemId;
                slot.ItemCount = 1;
                return true;
            }
        }
    }

    {   // �κ��丮�� ���� á�� ���
        GET_SINGLE(ChatManager)->AddMessage(L"�κ��丮�� ���� á���ϴ�.");
        return false;
    }
}

void Inventory::RemoveItem(ITEM& item)
{
    bool found = false;
    // �⺻ ����� ���� �Ұ�
    if (item.ItemId == 1 || item.ItemId == 2 || item.ItemId == 3)
        return;
    
    // �������� �̹� �����ϴ� ���
    for (auto& slot : _slots)
    {
        if (slot.ItemId == item.ItemId)
        {
            found = true;
            break;
        }
    }

    if (found == true)
    {
        item.ItemCount--; // ���� ����
        if (item.ItemCount <= 0)
        {
            item = {};
        }
    }
    // �������� �������� �ʴ� ���
    else
    {
        return;
    }
}

void Inventory::ChangeItem(ITEM& itemFrom, ITEM& itemTo)
{
    _temp = itemFrom;
    itemFrom = itemTo;
    itemTo = _temp;
}

int Inventory::GetIndex(int x, int y)
{
	return y * 8 + x;
}

vector<wstring> Inventory::findItemInfo(int itemID)
{
    for (auto& row : _itemTable)
    {
        if (row.empty())
            return {};

        int result;
        std::wistringstream(row[0]) >> result;

        if (result == itemID)
        {
            return row;
        }
    }

    // �����ϸ� ������ ��ȯ�ұ�
    return {};
}

int Inventory::findItemByName(wstring Name)
{
    for (auto& row : _itemTable)
    {
        if (row.empty())
            return -1;

        if (row[1] == Name)
        {
            return stoi(row[0]);
        }
    }

    return -1;
}


wstring Inventory::GetName(vector<wstring> row)
{
    return row[1];
}

wstring Inventory::GetKorName(vector<wstring> row)
{
    return row[2];
}

wstring Inventory::GetType(vector<wstring> row)
{
    return row[3];
}

wstring Inventory::GetDescription(vector<wstring> row)
{
    return row[4];
}

Sprite* Inventory::GetSprite(wstring wstr)
{
    return GET_SINGLE(ResourceManager)->GetSprite(wstr);
}
