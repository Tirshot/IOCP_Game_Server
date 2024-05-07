#include "pch.h"
#include "Inventory.h"
#include "Sprite.h"
#include "TextBox.h"
#include "QuickSlot.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "AlertBox.h"
#include "ResourceManager.h"
#include "ChatManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ItemManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"

Inventory::Inventory()
{
    _background = GET_SINGLE(ResourceManager)->GetSprite(L"Inventory");
    _itemSprite = GET_SINGLE(ResourceManager)->GetSprite(L"Sword");
}

Inventory::~Inventory()
{
    
}

void Inventory::BeginPlay()
{
    _invenRect = {};        // 485, 160, 770, 460
    {
        _invenRect.left = (int)_pos.x + 5;
        _invenRect.top = (int)_pos.y;
        _invenRect.right = _invenRect.left + 285;
        _invenRect.bottom = _invenRect.top + 335;
    }
    
    _dragRect = {};
    {
        _dragRect.left = (int)_pos.x;
        _dragRect.top = (int)_pos.y;
        _dragRect.right = _dragRect.left + 290;
        _dragRect.bottom = _dragRect.top + 35;
    }

    _slots.assign(40, { 0 });

    // _pos = 480, 125

    { // ������ �̸�
        wstring wstr = L"������ �̸�";
        _itemName = new TextBox(wstr);
        _itemName->SetPos({ _pos.x + 125, _pos.y + 225 });        // 605, 350
        _itemName->SetSize({ 155, 25 });
        _itemName->SetPadding(5, 5);
        _itemName->SetInitialPos(_itemName->GetPos());
        _itemName->SetFloating(true);
        AddChild(_itemName);
    }
    { // ������ ����
        wstring wstr = L"������ ����";
        _itemDescription = new TextBox(wstr);
        _itemDescription->SetPos({ _pos.x + 125, _pos.y + 225 });     // 605, 375
        _itemDescription->SetSize({ 155, 100 });
        _itemDescription->SetInitialPos(_itemDescription->GetPos());
        _itemDescription->SetPadding(5, 10);
        AddChild(_itemDescription);
    }
    { // ������ ����
        wstring wstr = L"����";
        _itemCount = new TextBox(wstr);
        _itemCount->SetPos({ _pos.x + 240, _pos.y + 225 });      // 710, 350
        _itemCount->SetSize({ 40, 25 });
        _itemCount->SetPadding(5, 5);
        _itemCount->SetInitialPos(_itemCount->GetPos());
        _itemCount->SetVisible(false);
        AddChild(_itemCount);
    }

    // �˾�
    {
        _alert = new AlertBox();
        if (_alert)
        {
            _alert->SetSize({ 300, 150 });
            _alert->SetPos({ 400, 300 });
            AddChild(_alert);
            _alert->AddParentDelegate(this, &Inventory::OnPopClickAcceptDelegate);
            _alert->SetVisible(false);
            _alert->SetInitialPos(_alert->GetPos());
        }
    }

    for (auto& child : _children)
        child->BeginPlay();

    SetInitialPos(GetPos());
    _initialized = true;
}

void Inventory::Tick()
{
    MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();

    // ���� 1ȸ�� ����
    if (_initialized)
    {
        // �⺻ ���� ����
        for (int i = 1; i < 5; i++)
            AddItem(i);

        // �׽�Ʈ�� ��� ����
        for (int i = 10; i < 25; i++)
            AddItem(i);

        // �� �⺻ ����
        ITEM* item1 = FindItemFromInventory(1);
        EquipItem(*item1);

        _initialized = false;
    }

    // Rect ��ġ �ʱ�ȭ
    {
        _invenRect.left = (int)_pos.x + 5;
        _invenRect.top = (int)_pos.y;
        _invenRect.right = _invenRect.left + 285;
        _invenRect.bottom = _invenRect.top + 335;
    }
    {
        _dragRect.left = (int)_pos.x;
        _dragRect.top = (int)_pos.y;
        _dragRect.right = _dragRect.left + 290;
        _dragRect.bottom = _dragRect.top + 35;
    }

    if (IsMouseOutRect(_invenRect) || IsMouseInRect(_dragRect))
    {   // ���콺�� ���� �ۿ� ����
        // ������ ���� �ʱ�ȭ
        _itemName->SetText(L"");
        _itemDescription->SetText(L"");
    }

    // ESC�� ������ �κ��丮 ����
    if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::ESC))
    {
        _visible = false;
        ResetPos();
    }

    // ������ ���� �ʱ�ȭ
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            // ������ �����ۿ� ���� �Ҵ�
            SetItemSlot(_slots[i + j * 8]);

            // �� ���Կ� rect �ʱ�ȭ
            auto& rect = _slots[i + j * 8].Rect;
            rect.left = _pos.x + 13 + i * (_slotSize + 6);
            rect.top = _pos.y + 45 + j * (_slotSize + 6);
            rect.right = rect.left + _slotSize;
            rect.bottom = rect.top + _slotSize;
        }
    }

    // ������ ����ȭ�Ǵ� �Ҹ��� ����
    if (myPlayer)
    {
        SetItemCount(4, myPlayer->info.arrows());
        SetItemCount(5, myPlayer->info.potion());
    }

    // UI ���� �ڵ� //
    if (_visible)
    {

        for (int i = 0; i < 4; i++)
        {
            // ��� ���� ����
            SetItemSlot(_equips[i]);
        }

        {
            // �� ��� ���Կ� RECT �Ҵ�
            _equips[0].Rect = RECT{ (int)_pos.x + 13, (int)_pos.y + 262, (int)_pos.x + 13 + 28, (int)_pos.y + 262 + 28 };    // ���� _pos.x + 13, _pos.y + 200
            _equips[1].Rect = RECT{ (int)_pos.x + 45, (int)_pos.y + 230, (int)_pos.x + 13 + 28, (int)_pos.y + 230 + 28 };    // ���
            _equips[2].Rect = RECT{ (int)_pos.x + 45, (int)_pos.y + 262, (int)_pos.x + 13 + 28, (int)_pos.y + 262 + 28 };    // ����
            _equips[3].Rect = RECT{ (int)_pos.x + 45, (int)_pos.y + 294, (int)_pos.x + 13 + 28, (int)_pos.y + 294 + 28 };    // ����
            _equips[4].Rect = RECT{ (int)_pos.x + 77, (int)_pos.y + 294, (int)_pos.x + 13 + 28, (int)_pos.y + 294 + 28 };    // �Ź�
        }

        for (auto& slot : _slots)
        {
            // ������ �巡�� �� ���
            if (IsMouseInRect(slot.Rect))
            {
                _mousePos = GET_SINGLE(InputManager)->GetMousePos();

                // ������ ����
                _itemName->SetText(slot.KorName);
                _itemDescription->SetText(slot.Description);

                // ������ ����
                if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::RightMouse))
                {
                    EquipItem(slot);
                }

                // �巡�� ����
                if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
                {
                    // �� ������ �巡�� �Ұ�
                    if (slot.ItemId <= 0)
                        return;

                    _selectedItem = &slot;
                }
                else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
                {
                    // �ٸ� �������� ���
                    if (_selectedItem != nullptr)
                    {
                        _destinatedItem = &slot;
                        ChangeItem(*_selectedItem, *_destinatedItem);
                        _selectedItem = nullptr;
                        return;
                    }
                }

                // �� ���Կ� ���
                PressToSetQuickItem(slot);

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
            if (GET_SINGLE(InputManager)->IsMouseOutRect(_invenRect)
                && GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
            {

                if (_selectedItem != nullptr)
                {
                    _deleteItem = _selectedItem;

                    int deleteItemID = _deleteItem->ItemId;

                    if (deleteItemID == 1 || deleteItemID == 2 || deleteItemID == 3)
                    {
                        _alert->SetText(L"�⺻ ����� ���� �� �����ϴ�.");
                        _alert->SetIcon(L"Alert");
                    }
                    else
                    {
                        _alert->SetText(L"�������� ��� �����ڽ��ϱ�?\n�ٽ� �ֿ� �� �����ϴ�.");
                        _alert->SetIcon(L"Danger");
                    }
                    _alert->SetVisible(true);
                    continue;
                }

                _selectedItem = nullptr;
                 
                //// �ܺ� Ŭ���� �κ��丮 ����
                //else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
                //{
                //    _visible = false;
                //}
            }
        }

        // ���԰� �κ��丮 �ٱ��� �ƴ� �������� ���
        if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
        {
            _selectedItem = nullptr;
        }

        // â �巡�׷� �̵�
        Panel::DragAndMove(&_dragRect);
        if (_isDragging)
        {
            // �κ��丮 ���� RECT �̵�
            _invenRect.left = (int)_pos.x + 5;
            _invenRect.top = (int)_pos.y;
            _invenRect.right = _invenRect.left + 285;
            _invenRect.bottom = _invenRect.top + 335;
        }
    }

    for (auto& child : _children)
        if (child->GetVisible())
            child->Tick();
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
            _slots[i].Sprite->GetPos().x,
            _slots[i].Sprite->GetPos().y,
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
            _equips[i].Sprite->GetPos().x,
            _equips[i].Sprite->GetPos().y,
            _equips[i].Sprite->GetSize().x,
            _equips[i].Sprite->GetSize().y,
            _equips[i].Sprite->GetTransparent());

        if (_slots[i].ItemId == 0)
            break;
    }

    for (auto& child : _children)
        if (child->GetVisible())
            child->Render(hdc);

    // �巡�� ���� ������
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
            _selectedItem->Sprite->GetPos().x,
            _selectedItem->Sprite->GetPos().y,
            _selectedItem->Sprite->GetSize().x,
            _selectedItem->Sprite->GetSize().y,
            _selectedItem->Sprite->GetTransparent());
    }

}

void Inventory::SetItemSlot(ITEM& slot)
{
    if (slot.ItemId <= 0)
        return;

    if (slot.ItemCount <= 0)
    {
        slot = {};
        return;
    }

    // ���� ����ȭ - �̹� ������ �Ҵ�� �������̶�� ��ŵ
    if (slot.Sprite != nullptr)
        return;

    // �̸��� Sprite, Rect, Description, Type �Ҵ�
    vector<wstring> ItemInfo = GET_SINGLE(ItemManager)->FindItemInfo(slot.ItemId);

    slot.Name = GET_SINGLE(ItemManager)->GetName(ItemInfo);
    slot.KorName = GET_SINGLE(ItemManager)->GetKorName(ItemInfo);
    slot.Description = GET_SINGLE(ItemManager)->GetDescription(ItemInfo);
    slot.Price = GET_SINGLE(ItemManager)->GetPrice(ItemInfo);
    slot.Type = GET_SINGLE(ItemManager)->GetType(ItemInfo);
    slot.SubType = GET_SINGLE(ItemManager)->GetSubType(ItemInfo);
    slot.Sprite = GET_SINGLE(ItemManager)->GetSprite(slot.Name);
}

void Inventory::SetEquipSlotRects()
{
    int _initialX = _pos.x + 13;
    int _initialY = _pos.y + 228;

    // �� ���Կ� Rect �Ҵ�
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            RECT Rect = { _initialX + (34 * i), _initialY + (34 * j), _initialX + (34 * i) + 22,  _initialY + (34 * j) + _slotSize };
            _equipRects.push_back(Rect);
        }
    }
}

void Inventory::SlotRectsPosUpdate(RECT* rect)
{
    rect->left = _pos.x + 13;
    rect->top = _pos.y + 45;
}

void Inventory::SyncItemToServer(int itemID, int counts)
{
    // ������ ����ȭ�Ǵ� �Ҹ�ǰ ���� -> ���� ���� �κ��丮�� �߰��� �ʿ䰡 ����
    {
        MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
        switch (itemID)
        {
        case 4: // Arrow
        {
            if (myPlayer)
            {
                int arrows = myPlayer->info.arrows();
                myPlayer->info.set_arrows(arrows + counts);

                if (counts == 0)
                    myPlayer->info.set_arrows(0);

                break;
            }
        }
        case 5: // Potion
        {
            if (myPlayer)
            {
                int potions = myPlayer->info.potion();
                myPlayer->info.set_potion(potions + counts);

                if (counts == 0)
                    myPlayer->info.set_potion(0);

                break;
            }
        }
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
            // ������ ������ ���� ������
            if (slot.Type == L"Wearable")
            {
                for (auto& slot : _slots)
                {
                    if (slot.ItemId == 0)
                    {
                        slot.ItemId = ItemId;
                        slot.ItemCount = 1;
                        SetItemSlot(slot);
                        return true;
                    }
                }
            }

            slot.ItemCount++; // ���� ����

            // �Ҹ� ������ ����ȭ
            SyncItemToServer(ItemId, 1);
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
                // ������ ������ ���� ������
                if (slot.Type == L"Wearable")
                {
                    for (auto& slot : _slots)
                    {
                        if (slot.ItemId == 0)
                        {
                            slot.ItemId = ItemId;
                            slot.ItemCount = 1;
                            SetItemSlot(slot);
                            return true;
                        }
                    }
                }
                slot.ItemId = ItemId;
                slot.ItemCount = 1;
                SyncItemToServer(ItemId, 1);
                SetItemSlot(slot);
                return true;
            }
        }
    }

    {   // �κ��丮�� ���� á�� ���
        GET_SINGLE(ChatManager)->AddMessage(L"�κ��丮�� ���� á���ϴ�.");
        return false;
    }
}

bool Inventory::AddItem(int ItemId, int ItemCount)
{
    bool found = false;
    int addCount = 0;

    // _slots ��ȸ
    for (auto& slot : _slots)
    {
        // �������� �̹� �����ϴ� ���
        if (slot.ItemId == ItemId)
        {
            // ������ ������ ���� ������
            if (slot.Type == L"Wearable")
            {
                for (int i = 0; i < ItemCount; i++)
                {
                    for (auto& slot : _slots)
                    {
                        if (addCount == ItemCount)
                            return true;

                        if (slot.ItemId == 0)
                        {
                            slot.ItemId = ItemId;
                            slot.ItemCount = 1;
                            addCount++;
                            SetItemSlot(slot);
                            continue;
                        }
                    }
                }
                return true;
            }

            // ���� ����
            slot.ItemCount += ItemCount;

            SyncItemToServer(ItemId, ItemCount);
            found = true;
            return true;
        }
    }

    // �������� �������� �ʴ� ���
    if (found == false)
    {
        for (auto& slot : _slots)
        {
            if (addCount == ItemCount)
                return true;

            if (slot.ItemId == 0)
            {
                // ������ ������ ���� ������
                if (slot.Type == L"Wearable")
                {
                    for (int i = 0; i < ItemCount; i++)
                    {
                        for (auto& slot : _slots)
                        {
                            if (slot.ItemId == 0)
                            {
                                slot.ItemId = ItemId;
                                slot.ItemCount = 1;
                                addCount++;
                                SetItemSlot(slot);
                                continue;
                            }
                        }
                    }
                    return true;
                }
                slot.ItemId = ItemId;
                slot.ItemCount = ItemCount;
                SetItemSlot(slot);

                SyncItemToServer(ItemId, ItemCount);
                return true;
            }
        }
    }

    {   // �κ��丮�� ���� á�� ���
        GET_SINGLE(ChatManager)->AddMessage(L"�κ��丮�� ���� á���ϴ�.");
        return false;
    }
}

bool Inventory::RemoveItem(ITEM* item)
{
    // �⺻ ����� ���� �Ұ�
    int itemId = item->ItemId;
    if (itemId == 1 || itemId == 2 || itemId == 3)
        return false;

    for (auto& slot : _slots)
    {
        if (slot == item)
        {
            slot.ItemCount--;

            SyncItemToServer(item->ItemId, -1);

            if (slot.ItemCount <= 0)
            {
                slot = {};
                return true;
            }
        }
    }
    return false;
}

bool Inventory::RemoveItem(ITEM* item, int ItemCount)
{
    // �⺻ ����� ���� �Ұ�
    int itemId = item->ItemId;
    if (itemId == 1 || itemId == 2 || itemId == 3)
        return false;

    for (auto& slot : _slots)
    {
        if (slot == item)
        {
            slot.ItemCount = 0;

            SyncItemToServer(item->ItemId, 0);

            if (slot.ItemCount <= 0)
            {
                slot = {};
                return true;
            }
        }
    }
    return false;
}

bool Inventory::RemoveItem(int itemId)
{
    bool found = false;
    // �⺻ ����� ���� �Ұ�
    if (itemId == 1 || itemId == 2 || itemId == 3)
        return false;
    
    // �������� �̹� �����ϴ� ���
    for (auto& slot : _slots)
    {
        if (slot.ItemId == itemId)
        {
            found = true;
        }

        if (found == true)
        {
            slot.ItemCount--; // ���� ����

            SyncItemToServer(itemId, -1);

            if (slot.ItemCount <= 0)
            {
                slot = {};
                return true;
            }
        }
    }

    // �������� ���� ���
    return false;
}

bool Inventory::RemoveItem(int itemId, int ItemCount)
{
    bool found = false;
    // �⺻ ����� ���� �Ұ�
    if (itemId == 1 || itemId == 2 || itemId == 3)
        return false;

    // �������� �̹� �����ϴ� ���
    for (auto& slot : _slots)
    {
        if (slot.ItemId == itemId)
        {
            found = true;
        }

        if (found == true)
        {
            // �����Ϸ��� �������� ������ ������ �纸�� ���� ���
            if (slot.ItemCount < ItemCount)
                return false;

            slot.ItemCount -= ItemCount; // ���� ����

            SyncItemToServer(itemId, -ItemCount);

            if (slot.ItemCount <= 0)
            {
                slot = {};
                return true;
            }
        }
    }

    // �������� ���� ���
    return false;
}

void Inventory::SetItemCount(int itemId, int ItemCount)
{
    ITEM* item = FindItemFromInventory(itemId);
    if (item)
    {
        item->ItemCount = ItemCount;
    }
    else
    {
        AddItem(itemId, ItemCount);
    }
}

void Inventory::ChangeItem(ITEM& itemFrom, ITEM& itemTo)
{
    ITEM temp = itemFrom;
    itemFrom = itemTo;
    itemTo = temp;
}

ITEM* Inventory::FindItemFromInventory(int itemId)
{
    for (auto& slot : _slots)
    {
        if (slot.ItemId == itemId)
            return &slot;
    }
    return nullptr;
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

    MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
    if (myPlayer == nullptr)
        return;

    if (item.SubType == L"Sword")
    {
        if (_equips[0].ItemId == item.ItemId)
            return;

        _equips[0] = item;
        myPlayer->SetWeaponType(Protocol::WEAPON_TYPE_SWORD);
        return;
    }
    else if (item.SubType == L"Bow")
    {
        if (_equips[0].ItemId == item.ItemId)
            return;

        _equips[0] = item;
        myPlayer->SetWeaponType(Protocol::WEAPON_TYPE_BOW);
        return;
    }
    else if (item.SubType == L"Staff")
    {
        if (_equips[0].ItemId == item.ItemId)
            return;

        _equips[0] = item;
        myPlayer->SetWeaponType(Protocol::WEAPON_TYPE_STAFF);
        return;
    }
    else if (item.SubType == L"Helmet")
    {
        if (_equips[1].ItemId == item.ItemId)
            return;

        _equips[1] = item;
        return;
    }
    else if (item.SubType == L"Armor")
    {
        if (_equips[2].ItemId == item.ItemId)
            return;

        _equips[2] = item;
        return;
    }
    else if (item.SubType == L"Pants")
    {
        if (_equips[3].ItemId == item.ItemId)
            return;

        _equips[3] = item;
        return;
    }
    else if (item.SubType == L"Boots")
    {
        if (_equips[4].ItemId == item.ItemId)
            return;

        _equips[4] = item;
        return;
    }
}

void Inventory::PressToSetQuickItem(ITEM& slot)
{
    // �� ���� ���
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

void Inventory::OnPopClickAcceptDelegate()
{
    if (RemoveItem(_deleteItem, _deleteItem->ItemCount))
        _deleteItem = nullptr;

    _isItemDropped = true;
}
