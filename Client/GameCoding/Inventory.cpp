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

    { // 아이템 이름
        wstring wstr = L"아이템 이름";
        _itemName = new TextBox(wstr);
        _itemName->SetPos({ 605, 350 });
        _itemName->SetSize({ 155, 25 });
        _itemName->SetPadding(5, 5);
        AddChild(_itemName);
    }
    { // 아이템 개수
        wstring wstr = L"개수";
        _itemCount = new TextBox(wstr);
        _itemCount->SetPos({ 710, 350 });
        _itemCount->SetSize({ 40, 25 });
        _itemCount->SetPadding(5, 5);
        _itemCount->SetVisible(false);
        AddChild(_itemCount);
    }
    { // 아이템 설명
        wstring wstr = L"아이템 설명";
        _itemDescription = new TextBox(wstr);
        _itemDescription->SetPos({ 605, 375 });
        _itemDescription->SetSize({ 155, 75 });
        _itemDescription->SetPadding(5, 5);
        AddChild(_itemDescription);
    }
    
    // 각 슬롯에 RECT 생성
    SetSlotRects();

    for (int i = 0; i < 5; i++)
        AddItem(i);

}

void Inventory::Tick()
{
    _mousePos = GET_SINGLE(InputManager)->GetMousePos();

    // 아이템 슬롯 초기화
    for (int i = 0; i < _slots.size(); i++)
    {
        // 보유한 아이템을 슬롯에 할당
        SetItemSlot(_slots[i]);

        // 각 슬롯에 RECT 할당
        _slots[i].Rect = _rects[i];
    }

    // 아이템 드래그 앤 드랍
    for (auto& slot : _slots)
    {
        if (IsMouseInRect(slot.Rect))
        {
            // 아이템 설명
            _itemName->SetText(slot.KorName);
            _itemDescription->SetText(slot.Description);

            // 드래그 시작
            if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
            {
                // 빈 슬롯은 드래그 불가
                if (slot.ItemId <= 0)
                    continue;

                _selectedItem = &slot;
            }
            else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
            {
                // 다른 슬롯으로 드랍
                if (_selectedItem != nullptr)
                {
                    _destinatedItem = &slot;
                    ChangeItem(*_selectedItem, *_destinatedItem);
                }
                _selectedItem = nullptr;
            }

            // 아이템 개수 표시
            if (slot.Type == L"Consumable")
            {
                _itemCount->SetVisible(true);
                _itemCount->SetText(to_wstring(slot.ItemCount) + L"개");
            }
            else
            {
                _itemCount->SetVisible(false);
            }
        }
        // 인벤토리 바깥으로 드랍
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

    // 이름과 Sprite, Rect, Description, Type 할당
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

    // 각 슬롯에 Rect 할당
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

    // _slots 순회
    for (auto& slot : _slots)
    {
        // 아이템이 이미 존재하는 경우
        if (slot.ItemId == ItemId)
        {
            slot.ItemCount++; // 수량 증가
            found = true;
            return true;
        }
    }

    // 아이템이 존재하지 않는 경우
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

    {   // 인벤토리가 가득 찼을 경우
        GET_SINGLE(ChatManager)->AddMessage(L"인벤토리가 가득 찼습니다.");
        return false;
    }
}

void Inventory::RemoveItem(ITEM& item)
{
    bool found = false;
    // 기본 무기는 제거 불가
    if (item.ItemId == 1 || item.ItemId == 2 || item.ItemId == 3)
        return;
    
    // 아이템이 이미 존재하는 경우
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
        item.ItemCount--; // 수량 감소
        if (item.ItemCount <= 0)
        {
            item = {};
        }
    }
    // 아이템이 존재하지 않는 경우
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

    // 실패하면 무엇을 반환할까
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
