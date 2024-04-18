#include "pch.h"
#include "Inventory.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "FileManager.h"
#include "ChatManager.h"
#include "InputManager.h"
#include "ItemManager.h"
#include "TextBox.h"
#include "QuickSlot.h"

Inventory::Inventory()
{

}

Inventory::~Inventory()
{
    
}

void Inventory::BeginPlay()
{
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
    SetEquipSlotRects();

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

    for (int i = 0; i < 4; i++)
    {
        // 장비 슬롯 연동
        SetItemSlot(_equips[i]);
    }

    {
        // 각 장비 슬롯에 RECT 할당
        _equips[0].Rect = _equipRects[3];    // 무기
        _equips[1].Rect = _equipRects[1];    // 헬멧
        _equips[2].Rect = _equipRects[4];    // 몸통
        _equips[3].Rect = _equipRects[7];    // 바지
        _equips[4].Rect = _equipRects[8];    // 신발
    }
    
    // 아이템 드래그 앤 드랍
    for (auto& slot : _slots)
    {
        if (IsMouseInRect(slot.Rect))
        {
            // 아이템 설명
            _itemName->SetText(slot.KorName);
            _itemDescription->SetText(slot.Description);

            // 아이템 장착
            if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::RightMouse))
            {
                EquipItem(slot);
            }

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
                _destinatedItem = nullptr;
                _temp = {};
            }

            // 퀵 슬롯에 등록
            PressToSetQuickItem(slot);

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
                    if (RemoveItem(_selectedItem->ItemId))
                        _selectedItem = nullptr;
                }
            }
        }
    } // 슬롯과 인벤토리 바깥이 아닌 영역으로 드랍
    if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
    {
        _selectedItem = nullptr;
    }
}

void Inventory::Render(HDC hdc)
{
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

    for (int i = 0; i < 5; i++)
    {
        if (_equips[i].Sprite == nullptr)
            continue;

        float spritePosX = _equips[i].Rect.left;
        float spritePosY = _equips[i].Rect.top - 2;

        ::TransparentBlt(hdc,
            spritePosX,
            spritePosY,
            _slotSize,
            _slotSize,
            _equips[i].Sprite->GetDC(),
            0,
            0,
            _equips[i].Sprite->GetSize().x,
            _equips[i].Sprite->GetSize().y,
            _equips[i].Sprite->GetTransparent());

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
    vector<wstring> ItemInfo = GET_SINGLE(ItemManager)->FindItemInfo(slot.ItemId);

    slot.Name = GET_SINGLE(ItemManager)->GetName(ItemInfo);
    slot.KorName = GET_SINGLE(ItemManager)->GetKorName(ItemInfo);
    slot.Description = GET_SINGLE(ItemManager)->GetDescription(ItemInfo);
    slot.Type = GET_SINGLE(ItemManager)->GetType(ItemInfo);
    slot.SubType = GET_SINGLE(ItemManager)->GetSubType(ItemInfo);
    slot.Sprite = GET_SINGLE(ItemManager)->GetSprite(slot.Name);
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
            RECT Rect = { _initialX + (34 * i), _initialY + (34 * j), _initialX + (34 * i) + _slotSize,  _initialY + (34 * j) + _slotSize };
            _rects.push_back(Rect);
        }
    }
}

void Inventory::SetEquipSlotRects()
{
    int _initialX = _pos.x + 13;
    int _initialY = _pos.y + 228;

    // 각 슬롯에 Rect 할당
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            RECT Rect = { _initialX + (34 * i), _initialY + (34 * j), _initialX + (34 * i) + 22,  _initialY + (34 * j) + _slotSize };
            _equipRects.push_back(Rect);
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
                auto ItemInfo = GET_SINGLE(ItemManager)->FindItemInfo(slot.ItemId);
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

bool Inventory::RemoveItem(int itemId)
{
    bool found = false;
    // 기본 무기는 제거 불가
    if (itemId == 1 || itemId == 2 || itemId == 3)
        return false;
    
    // 아이템이 이미 존재하는 경우
    for (auto& slot : _slots)
    {
        if (slot.ItemId == itemId)
        {
            found = true;
        }

        if (found == true)
        {
            slot.ItemCount--; // 수량 감소
            if (slot.ItemCount <= 0)
            {
                slot = {};
                return true;
            }
        }
    }

    // 아이템이 없는 경우
    return false;
}

void Inventory::ChangeItem(ITEM& itemFrom, ITEM& itemTo)
{
    _temp = itemFrom;
    itemFrom = itemTo;
    itemTo = _temp;
}

ITEM* Inventory::GetEquippedItem(wstring wstr)
{
    if (wstr == L"Weapon")
        return &_equips[0];

    if (wstr == L"Helmet")
        return &_equips[1];

    if (wstr == L"Armor")
        return &_equips[2];

    if (wstr == L"Pants")
        return &_equips[3];

    if (wstr == L"Boots")
        return &_equips[4];
}

void Inventory::EquipItem(ITEM& item)
{
    if (item.Type != L"Wearable")
        return;

    if (item.SubType == L"Weapon")
    {
        _equips[0] = item;
        return;
    }
    else if (item.SubType == L"Helmet")
    {
        _equips[1] = item;
        return;
    }
    else if (item.SubType == L"Armor")
    {
        _equips[2] = item;
        return;
    }
    else if (item.SubType == L"Pants")
    {
        _equips[3] = item;
        return;
    }
    else if (item.SubType == L"Boots")
    {
        _equips[4] = item;
        return;
    }
}

void Inventory::PressToSetQuickItem(ITEM& slot)
{
    // 퀵 슬롯 등록
    if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_1))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(&slot, 1);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_2))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(&slot, 2);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_3))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(&slot, 3);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_4))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(&slot, 4);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_5))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(&slot, 5);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_6))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(&slot, 6);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_7))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(&slot, 7);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_8))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(&slot, 8);
    }
}
