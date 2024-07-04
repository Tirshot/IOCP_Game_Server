#include "pch.h"
#include "Inventory.h"
#include "Sprite.h"
#include "TextBox.h"
#include "QuickSlot.h"
#include "DevScene.h"
#include "MyPlayer.h"
#include "AlertBox.h"
#include "ShopUI.h"
#include "ResourceManager.h"
#include "ChatManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ItemManager.h"
#include "NetworkManager.h"
#include "ClientPacketHandler.h"
#include "TimeManager.h"

Inventory::Inventory()
{
    _background = GET_SINGLE(ResourceManager)->GetSprite(L"Inventory");
    _itemSprite = GET_SINGLE(ResourceManager)->GetSprite(L"Sword");
    _descriptionSprite = GET_SINGLE(ResourceManager)->GetSprite(L"PopBackground");

    _slots.assign(40, { 0 });
    _equips.assign(5, { RECT(), ITEM() });
}

Inventory::~Inventory()
{
    _slots.clear();
    _equips.clear();
    _rects.clear();
    _equipRects.clear();

    _owner = nullptr;
    _itemName = nullptr;
    _itemCount = nullptr;
    _itemDescription = nullptr;
    _background = nullptr;
    _itemSprite = nullptr;
    _descriptionSprite = nullptr;
}

void Inventory::BeginPlay()
{
    Super::BeginPlay();

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

    _equipRect = {};
    {
        _equipRect.left = (int)_pos.x + 5;
        _equipRect.top = (int)_pos.y + 220;
        _equipRect.right = _equipRect.left + 110;
        _equipRect.bottom = _equipRect.top + 110;
    }

    // _pos = 480, 125

    { // 아이템 설명
        wstring wstr = L"아이템 설명";
        _itemDescription = make_shared<TextBox>(wstr);
        _itemDescription->SetPos({ _pos.x + 125, _pos.y + 225 });     // 605, 375
        _itemDescription->SetSize({ 155, 100 });
        _itemDescription->SetInitialPos(_itemDescription->GetPos());
        _itemDescription->SetPadding(5, 10);
        AddChild(_itemDescription);
    }

    { // 아이템 이름
        wstring wstr = L"아이템 이름";
        _itemName = make_shared<TextBox>(wstr);
        _itemName->SetPos({ _pos.x + 125, _pos.y + 225 });        // 605, 350
        _itemName->SetSize({ 155, 25 });
        _itemName->SetPadding(5, 5);
        _itemName->SetInitialPos(_itemName->GetPos());
        _itemName->SetFloating(true);
        AddChild(_itemName);
    }

    { // 아이템 개수
        wstring wstr = L"개수";
        _itemCount = make_shared<TextBox>(wstr);
        _itemCount->SetPos({ _pos.x + 240, _pos.y + 225 });      // 710, 350
        _itemCount->SetSize({ 40, 25 });
        _itemCount->SetPadding(5, 5);
        _itemCount->SetInitialPos(_itemCount->GetPos());
        _itemCount->SetVisible(false);
        AddChild(_itemCount);
    }

    SetInitialPos(GetPos());

    // 아이템 슬롯 초기화
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            // 각 슬롯에 rect 초기화
            if (_slots[i + j * 8] == nullptr)
            {
                RECT rect;
                rect.left = _pos.x + 13 + i * (_slotSize + 6);
                rect.top = _pos.y + 45 + j * (_slotSize + 6);
                rect.right = rect.left + _slotSize;
                rect.bottom = rect.top + _slotSize;

                _slots[i + j * 8] = make_shared<ITEM>();
                _slots[i + j * 8]->Rect = rect;
            }
        }
    }

    // 기본 무기 지급
    for (int i = 1; i < 4; i++)
        AddItem(i);

    _initialized = true;
}

void Inventory::Tick()
{
    Super::Tick();

    auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
    if (myPlayer)
    {
        _owner = myPlayer;
    }

    // 최초 1회만 실행
    if (_initialized)
    {
        // 검 기본 장착
        shared_ptr<ITEM> item1 = FindItemFromInventory(1);
        EquipItem(item1);

        _initialized = false;
    }

    // Rect 위치 초기화
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
    {
        _equipRect.left = (int)_pos.x + 5;
        _equipRect.top = (int)_pos.y + 220;
        _equipRect.right = _equipRect.left + 110;
        _equipRect.bottom = _equipRect.top + 110;
    }

    //// 각 장비 슬롯에 RECT 할당
    {
        _equips[0].first = (RECT{ (int)_pos.x + 13, (int)_pos.y + 262, (int)_pos.x + 13 + 28, (int)_pos.y + 262 + 28 }); // 무기 _pos.x + 13, _pos.y + 200
        _equips[1].first = (RECT{ (int)_pos.x + 45, (int)_pos.y + 230, (int)_pos.x + 45 + 28, (int)_pos.y + 230 + 28 }); // 헬멧
        _equips[2].first = (RECT{ (int)_pos.x + 45, (int)_pos.y + 262, (int)_pos.x + 45 + 28, (int)_pos.y + 262 + 28 }); // 몸통
        _equips[3].first = (RECT{ (int)_pos.x + 45, (int)_pos.y + 294, (int)_pos.x + 45 + 28, (int)_pos.y + 294 + 28 }); // 바지
        _equips[4].first = (RECT{ (int)_pos.x + 77, (int)_pos.y + 294, (int)_pos.x + 77 + 28, (int)_pos.y + 294 + 28 }); // 신발
    }

    if (IsMouseOutRect(_invenRect) || IsMouseInRect(_dragRect))
    {   // 마우스가 슬롯 밖에 있음
        // 아이템 설명 초기화
        _itemName->SetText(L"");
        _itemCount->SetText(L"");
        _itemDescription->SetText(L"");
    }

    // ESC를 누르면 인벤토리 끄기
    if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::ESC))
    {
        // 팝업이 켜져있으면 끌 수 없음
        if (IsChildPopUpVisible() == false)
        {
            ResetPos();
            SetVisible(false);
        }
    }

    // 아이템 슬롯 초기화
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            RECT rect;
            rect.left = _pos.x + 13 + i * (_slotSize + 6);
            rect.top = _pos.y + 45 + j * (_slotSize + 6);
            rect.right = rect.left + _slotSize;
            rect.bottom = rect.top + _slotSize;

            _slots[i + j * 8]->Rect = rect;
        }
    }

    SyncUseableItem();

    // popup이 있으면 일시정지
    if (_pause)
        return;

    _deleteItem = nullptr;

    // UI 관련 코드 //
    if (_visible)
    {
        // 창 드래그로 이동
        Panel::DragAndMove(_dragRect);
        if (_isDragging)
        {
            // 인벤토리 영역 RECT 이동
            _invenRect.left = (int)_pos.x + 5;
            _invenRect.top = (int)_pos.y;
            _invenRect.right = _invenRect.left + 285;
            _invenRect.bottom = _invenRect.top + 335;
        }

        // 자식의 팝업이 켜져있을 때 상호작용 불가
        if (IsChildPopUpVisible())
            return;

        if (IsAnyPopUpVisible())
            return;

        // 아이템 슬롯 초기화
        for (int j = 0; j < 5; j++)
        {
            for (int i = 0; i < 8; i++)
            {
                if (_slots[i + j * 8] == nullptr)
                    return;

                RECT rect = {};
                rect.left = _pos.x + 13 + i * (_slotSize + 6);
                rect.top = _pos.y + 45 + j * (_slotSize + 6);
                rect.right = rect.left + _slotSize;
                rect.bottom = rect.top + _slotSize;

                _slots[i + j * 8]->Rect = rect;
            }
        }

        // 인벤토리
        for (auto& slot : _slots)
        {
            if (slot->ItemCount <= 0)
            {
                slot->Reset();
            }

            // 아이템 드래그 앤 드랍
            if (IsMouseInRect(slot->Rect) &&
                IsOverlappedWithVisibleUIRect(slot->Rect) == false)
            {
                _mousePos = GET_SINGLE(InputManager)->GetMousePos();

                // 아이템 설명
                _itemName->SetText(slot->KorName);
                _itemDescription->SetText(slot->Description);

                if (slot->Type == L"Consumable" || slot->Type == L"ETC")
                {
                    _itemCount->SetVisible(true);
                    _itemCount->SetText(format(L"{0}개", slot->ItemCount));
                }
                else
                {
                    _itemCount->SetVisible(false);
                    _itemCount->SetText(L"");
                }

                // 아이템 장착
                if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::RightMouse))
                {
                    EquipItem(slot);
                    return;
                }

                // 드래그 시작
                if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
                {
                    // 빈 슬롯은 드래그 불가
                    if (slot->ItemId <= 0)
                        return;

                    _selectedItem = slot;
                }
                else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
                {
                    // 다른 슬롯으로 드랍
                    if (_selectedItem != nullptr)
                    {
                        _destinatedItem = slot;

                        if (_selectedItem == _destinatedItem)
                        {
                            _selectedItem = nullptr;
                            return;
                        }

                        // 장비창의 아이템을 인벤토리의 아이템에게로 드래그 & 드랍 했을 때
                        if (_isEquipedItem == true)
                        {
                            // 아이템 교체
                            ChangeItem(*_selectedItem, *_destinatedItem);
                            _selectedItem = nullptr;
                            _isEquipedItem = false;
                            return;
                        }
                        else
                        {
                            ChangeItem(*_selectedItem, *_destinatedItem);
                            _selectedItem = nullptr;
                            return;
                        }
                    }
                }

                // 퀵 슬롯에 등록
                PressToSetQuickItem(*slot);

                // 아이템 개수 표시
                if (slot->Type == L"Consumable" || slot->Type == L"ETC")
                {
                    _itemCount->SetVisible(true);
                    _itemCount->SetText(to_wstring(slot->ItemCount) + L"개");
                }
                else
                {
                    _itemCount->SetVisible(false);
                }
            }
            // 슬롯에서 장비창으로 드래그
            else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
            {
                // 장비창으로 드래그 하면 장착
                if (IsMouseInRect(_equipRect))
                {
                    EquipItem(_selectedItem);
                    _selectedItem = nullptr;
                    break;
                }
            }

            // 인벤토리 바깥으로 드랍
            if (GET_SINGLE(InputManager)->IsMouseOutRect(_invenRect)
                && GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
            {
                // 장비템을 드래그하지 않을 때만
                if (_isEquipedItem == false)
                {
                // Shop UI가 켜져있으면 판매
                    if (_selectedItem != nullptr)
                    {
                        auto scene = GET_SINGLE(SceneManager)->GetDevScene();
                        if (scene)
                        {
                            shared_ptr<ShopUI> shop = scene->FindUI<ShopUI>(scene->GetUIs());
                            if (shop && shop->GetVisible())
                            {
                                shop->SellItemToShop(_selectedItem);
                                _selectedItem = nullptr;
                                break;
                            }
                        }

                        if (_deleteItem)
                            break;

                        // 일반적으로는 아이템 버리기
                        _deleteItem = _selectedItem;
                        _selectedItem = nullptr;

                        int deleteItemID = _deleteItem->ItemId;

                        if (deleteItemID == 1 || deleteItemID == 2 || deleteItemID == 3)
                        {
                            shared_ptr<AlertBox> alert = MakeAlertBox({ GWinSizeX / 2,GWinSizeY / 2 }, { 300,150 }, &Inventory::OnPopClickAcceptDelegate, false);
                            SetPause(true);

                            alert->SetText(L"기본 무기는 버릴 수 없습니다.");
                            alert->SetIcon(L"Alert");
                        }
                        else
                        {
                            shared_ptr<AlertBox> alert = MakeAlertBox({ GWinSizeX / 2,GWinSizeY / 2 }, { 300,150 }, &Inventory::OnPopClickAcceptDelegate);
                            SetPause(true);

                            alert->SetText(L"아이템을 모두 버리겠습니까?\n다시 주울 수 없습니다.");
                            alert->SetIcon(L"Danger");
                        }
                    }
                    continue;
                }
            }

            // 슬롯과 인벤토리 바깥이 아닌 영역으로 드랍
            if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse)
                && IsMouseOutRect(_equipRect))
            {
                //_selectedItem = nullptr;
            }
        }

        // 장비창
        for (auto& slot : _equips)
        {
            // 아이템 드래그 앤 드랍
            if (IsMouseInRect(slot.first))
            {
                _mousePos = GET_SINGLE(InputManager)->GetMousePos();

                if (slot.second.Sprite == nullptr)
                    break;

                // 아이템 설명
                _itemName->SetText(slot.second.KorName);
                _itemDescription->SetText(slot.second.Description);

                // 아이템 장착 해제
                if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::RightMouse))
                {
                    if (slot.second.Sprite == nullptr)
                        break;

                    if (slot.second.ItemId <= 3)
                        break;

                    AddItem(slot.second.ItemId);
                    slot.second.Reset();
                    SyncEquips(_owner->GetObjectID(), false);
                }

                // 드래그 시작
                if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
                {
                    if (slot.second.Sprite == nullptr)
                        break;

                    // 빈 슬롯은 드래그 불가
                    if (slot.second.ItemId <= 0)
                        return;

                    _isEquipedItem = true;
                    _selectedItem = make_shared<ITEM>(slot.second);
                }

                // 인벤토리 바깥으로 드랍 -> 아이템 장착 해제
                if (GET_SINGLE(InputManager)->IsMouseOutRect(_equipRect)
                    && GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
                {
                    if (_selectedItem == nullptr)
                        continue;

                    if (_selectedItem->ItemId <= 3)
                        continue;

                    AddItem(_selectedItem->ItemId);
                    for (auto& equip : _equips)
                    {
                        if (equip.second.ItemId == _selectedItem->ItemId)
                        {
                            equip.second.Reset();
                        }
                    }
                    SyncEquips(_owner->GetObjectID(), false);
                    _isEquipedItem = false;
                    _selectedItem = nullptr;
                }
            }
        }
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

    ::StretchBlt(hdc,
        _pos.x + 125,
        _pos.y + 225,
        155,
        100,
        _descriptionSprite->GetDC(),
        50,
        50,
        100,
        100,
        SRCCOPY);

    for (int i = 0; i < 40; i++)
    {
        if (_slots[i]->Sprite == nullptr)
            continue;

        float spritePosX = _slots[i]->Rect.left;
        float spritePosY = _slots[i]->Rect.top - 2;

        ::TransparentBlt(hdc,
            spritePosX,
            spritePosY,
            _slotSize,
            _slotSize,
            _slots[i]->Sprite->GetDC(),
            _slots[i]->Sprite->GetPos().x,
            _slots[i]->Sprite->GetPos().y,
            _slots[i]->Sprite->GetSize().x,
            _slots[i]->Sprite->GetSize().y,
            _slots[i]->Sprite->GetTransparent());

        if (_slots[i]->ItemId == 0)
            break;
    }

    for (int i = 0; i < 5; i++)
    {
        if (_equips[i].second.Sprite == nullptr)
            continue;

        float spritePosX = _equips[i].first.left;
        float spritePosY = _equips[i].first.top - 2;

        ::TransparentBlt(hdc,
            spritePosX,
            spritePosY,
            _slotSize,
            _slotSize,
            _equips[i].second.Sprite->GetDC(),
            _equips[i].second.Sprite->GetPos().x,
            _equips[i].second.Sprite->GetPos().y,
            _equips[i].second.Sprite->GetSize().x,
            _equips[i].second.Sprite->GetSize().y,
            _equips[i].second.Sprite->GetTransparent());
    }

    Super::Render(hdc);

    // 드래그 중인 아이템
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

shared_ptr<AlertBox> Inventory::MakeAlertBox(Vec2 pos, Vec2Int size, void (Inventory::* func)(), bool twoButtons)
{
    // 팝업
    shared_ptr<AlertBox> alert = make_shared<AlertBox>();
    if (alert)
    {
        // AlertBox 초기화
        alert->SetSize(size);
        alert->SetPos({ pos.x, pos.y });
        alert->SetVisible(true);
        alert->SetInitialPos(alert->GetPos());
        alert->MakeAcceptButton();

        if (twoButtons)
            alert->MakeDenyButton();

        alert->AddParentDelegate(this, func);
        alert->BeginPlay();
    }

    AddChild(alert);

    return alert;
}

void Inventory::SetItemSlot(ITEM& slot)
{
    if (slot.ItemId <= 0)
        return;

    auto item = GET_SINGLE(ItemManager)->GetItem(slot.ItemId);
    if (item.Sprite == nullptr)
        return;

    if (slot.ItemCount <= 0)
    {
        RECT rect = slot.Rect;
        slot = {};
        slot.Rect = rect;
        return;
    }

    // 성능 최적화 - 이미 정보가 할당된 아이템이라면 스킵
    if (slot.Sprite != nullptr)
        return;

    // 이름과 Sprite, Rect, Description, Type 할당
    vector<wstring> ItemInfo = GET_SINGLE(ItemManager)->FindItemInfo(slot.ItemId);

    slot.Name = GET_SINGLE(ItemManager)->GetName(ItemInfo);
    slot.KorName = GET_SINGLE(ItemManager)->GetKorName(ItemInfo);
    slot.Description = GET_SINGLE(ItemManager)->GetDescription(ItemInfo);
    slot.Price = GET_SINGLE(ItemManager)->GetPrice(ItemInfo);
    slot.Type = GET_SINGLE(ItemManager)->GetType(ItemInfo);
    slot.SubType = GET_SINGLE(ItemManager)->GetSubType(ItemInfo);
    slot.Sprite = GET_SINGLE(ItemManager)->GetSprite(slot.Name);
    slot.MaxCount = GET_SINGLE(ItemManager)->GetMaxCounts(ItemInfo);
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
            RECT Rect = RECT({ _initialX + (34 * i), _initialY + (34 * j), _initialX + (34 * i) + 22,  _initialY + (34 * j) + _slotSize });
            _equipRects.push_back(Rect);
        }
    }
}

void Inventory::SlotRectsPosUpdate(RECT rect)
{
    rect.left = _pos.x + 13;
    rect.top = _pos.y + 45;
}

void Inventory::SyncUseableItem()
{
    auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
    if (myPlayer)
    {
        auto arrows = myPlayer->info.arrows();
        auto potions = myPlayer->info.potion();

        auto arrow = FindItemFromInventory(4);
        auto potion = FindItemFromInventory(5);

        if (arrow)
        {
            arrow->ItemCount = arrows;
        }
        else
        {
            AddItem(4, arrows);
            myPlayer->info.set_arrows(arrows);
        }

        if (potion)
        {
            potion->ItemCount = potions;
        }
        else
        {
            AddItem(5, potions);
            myPlayer->info.set_potion(potions);
        }
    }
}

void Inventory::AddUseableItemToServer(int itemID, int counts)
{
    // 서버와 동기화되는 소모품 관리 -> 서버 관리 인벤토리에 추가할 필요가 있음
    auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
    if (myPlayer == nullptr)
        return;

    switch (itemID)
    {
    case 4: // Arrow
    {
        int arrows = myPlayer->info.arrows();
        if (counts == 0)
            myPlayer->info.set_arrows(0);
        else
            myPlayer->info.set_arrows(arrows + counts);

        break;
    }

    case 5: // Potion
    {
        int potions = myPlayer->info.potion();
        if (counts == 0)
            myPlayer->info.set_potion(0);
        else
            myPlayer->info.set_potion(potions + counts);

        break;
    }

    default:
        break;
    }
}

void Inventory::AddItemToServer(int itemID, int counts)
{
    int objectID = -1;
    objectID = GET_SINGLE(SceneManager)->GetMyPlayerId();

    if (objectID == -1 || _initialized == true)
        return;

    auto iteminfo = GET_SINGLE(ItemManager)->FindItemInfo(itemID);
    auto itemType = GET_SINGLE(ItemManager)->GetType(iteminfo);

    Protocol::ITEM_TYPE protoItemType = Protocol::ITEM_TYPE_NONE;

    if (itemType == L"Wearable")
    {
        protoItemType = Protocol::ITEM_TYPE_WEARABLE;
    }
    else if (itemType == L"Consumable")
    {
        protoItemType = Protocol::ITEM_TYPE_CONSUMABLE;
    }
    else if (itemType == L"ETC")
    {
        protoItemType = Protocol::ITEM_TYPE_ETC;
    }

    int itemIndex = FindItemFromInventory(itemID)->index;

    SendBufferRef sendBuffer = ClientPacketHandler::Make_C_AddItem(objectID, itemID, counts, protoItemType, itemIndex);
    GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
}

void Inventory::AutoSyncInventory()
{
    // 인벤토리 정보 서버로 송신
    for (auto& slot : _slots)
    {
        AddItemToServer(slot->ItemId, slot->ItemCount);
    }
}

void Inventory::SyncEquips(int itemID, bool equip)
{
    if (_owner == nullptr)
        return;

    if (equip)
    {
        SendBufferRef sendBuffer = ClientPacketHandler::Make_C_EquipItem(_owner->GetObjectID(), itemID);
        GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
    }
    else
    {
        SendBufferRef sendBuffer = ClientPacketHandler::Make_C_EquipItem(_owner->GetObjectID(), itemID , false);
        GET_SINGLE(NetworkManager)->SendPacket(sendBuffer);
    }
}

bool Inventory::AddItem(shared_ptr<ITEM> item)
{
    bool found = false;

    if (item == nullptr)
        return false;

    int ItemId = item->ItemId;

    if (item->ItemId <= 3)
        return false;

    int emptySlot = 0;
    wstring itemType = GET_SINGLE(ItemManager)->GetItem(item->ItemId).Type;
    const auto itemMaxCount = GET_SINGLE(ItemManager)->GetItem(ItemId).MaxCount;

    for (auto& slot : _slots)
    {
        if (slot == nullptr)
            return false;

        if (slot->ItemId == 0)
        {
            emptySlot++;
            continue;
        }

        if (slot->ItemId == ItemId)
            found = true;
    }

    // _slots 순회
    for (auto& slot : _slots)
    {
        // 아이템이 이미 존재하는 경우
        if (found)
        {
            // 장비류는 슬롯을 따로 차지함
            if (itemType == L"Wearable")
            {
                if (emptySlot < 1)
                    return false;

                // 빈 슬롯을 앞에서부터 찾아 추가
                for (auto& slot : _slots)
                {
                    if (slot == nullptr)
                        return false;

                    if (slot->ItemId == 0)
                    {
                        slot->Description = item->Description;
                        slot->ItemCount = 1;
                        slot->MaxCount = item->MaxCount;
                        slot->ItemId = ItemId;
                        slot->KorName = item->KorName;
                        slot->Name = item->Name;
                        slot->Price = item->Price;
                        slot->Sprite = item->Sprite;
                        slot->SubType = item->SubType;
                        slot->Type = item->Type;

                        AddItemToServer(ItemId, 1);
                        SetItemSlot(*slot.get());
                        return true;
                    }
                }
            }
            else if (itemType == L"Consumable")
            {
                for (auto& slot : _slots)
                {
                    if (slot->ItemId == ItemId)
                    {
                        slot->ItemCount++; // 수량 증가
                        // 소모성 아이템 동기화
                        //AddUseableItemToServer(ItemId, 1);
                        SetItemSlot(*slot.get());
                        return true;
                    }
                }
            }
            else // 스택이 되며, 장비와 소모품이 아닌 아이템
            {
                for (auto& slot : _slots)
                {
                    slot->ItemCount++; // 수량 증가
                    AddItemToServer(ItemId, 1);
                    SetItemSlot(*slot.get());
                    return true;
                }
            }
        }
        // 아이템이 존재하지 않는 경우
        else if (found == false)
        {
            if (emptySlot < 1)
                return false;

            for (auto& slot : _slots)
            {
                if (slot == nullptr)
                    return false;

                if (slot->ItemId == 0)
                {
                    slot->Description = item->Description;
                    slot->ItemCount = 1;
                    slot->MaxCount = item->MaxCount;
                    slot->ItemId = ItemId;
                    slot->KorName = item->KorName;
                    slot->Name = item->Name;
                    slot->Price = item->Price;
                    slot->Sprite = item->Sprite;
                    slot->SubType = item->SubType;
                    slot->Type = item->Type;
                    AddItemToServer(ItemId, 1);
                    SetItemSlot(*slot.get());
                    return true;
                }
            }
        }
    }

    {   // 인벤토리가 가득 찼을 경우
        GET_SINGLE(ChatManager)->AddMessage(L"인벤토리가 가득 찼습니다.");
        return false;
    }
}

bool Inventory::AddItem(int ItemId)
{
    if (ItemId <= 0)
        return false;

    bool found = false;
    int emptySlot = 0;
    wstring itemType = GET_SINGLE(ItemManager)->GetItem(ItemId).Type;
    const auto itemMaxCount = GET_SINGLE(ItemManager)->GetItem(ItemId).MaxCount;

    for (auto& slot : _slots)
    {
        if (slot == nullptr)
            return false;

        if (slot->ItemId == 0)
        {
            emptySlot++;
            continue;
        }

        if (slot->ItemId == ItemId)
            found = true;
    }

    // _slots 순회
    for (auto& slot : _slots)
    {
        // 아이템이 이미 존재하는 경우
        if (found)
        {
            // 장비류는 슬롯을 따로 차지함
            if (itemType == L"Wearable")
            {
                if (emptySlot < 1)
                    return false;

                int index = 0;

                // 빈 슬롯을 앞에서부터 찾아 추가
                for (auto& slot : _slots)
                {
                    if (slot == nullptr)
                        return false;

                    if (slot->ItemId == 0)
                    {
                        slot->index = index;
                        slot->ItemId = ItemId;
                        slot->ItemCount = 1;
                        AddItemToServer(ItemId, 1);
                        SetItemSlot(*slot.get());
                        return true;
                    }
                    index++;
                }
            }
            else if (itemType == L"Consumable")
            {
                int index = 0;

                for (auto& slot : _slots)
                {
                    if (slot->ItemId == ItemId)
                    {
                        // 추가한 양이 최대 개수보다 적을 때
                        if (slot->ItemCount + 1 <= itemMaxCount)
                        {
                            slot->index = index;
                            slot->ItemCount++; // 수량 증가
                            // 소모성 아이템 동기화
                            AddUseableItemToServer(ItemId, 1);
                            SetItemSlot(*slot.get());
                            return true;
                        }
                    }
                    index++;
                }
            }
            else // 스택이 되며, 장비와 소모품이 아닌 아이템
            {
                int index = 0;

                for (auto& slot : _slots)
                {
                    if (slot->ItemId == ItemId)
                    {
                        // 추가한 양이 최대 개수보다 적을 때
                        if (slot->ItemCount + 1 <= itemMaxCount)
                        {
                            slot->index = index;
                            slot->ItemCount++; // 수량 증가
                            AddItemToServer(ItemId, 1);
                            SetItemSlot(*slot.get());
                            return true;
                        }
                    }
                    index++;
                }
            }
        }
        // 아이템이 존재하지 않는 경우
        else if (found == false)
        {
            if (emptySlot < 1)
                return false;

            int index = 0;

            for (auto& slot : _slots)
            {
                if (slot == nullptr)
                    return false;

                if (slot->ItemId == 0)
                {
                    slot->index = index;
                    slot->ItemId = ItemId;
                    slot->ItemCount += 1;
                    AddUseableItemToServer(ItemId, 1);
                    AddItemToServer(ItemId, 1);
                    SetItemSlot(*slot.get());
                    return true;
                }
                index++;
            }
        }
    }

    {   // 인벤토리가 가득 찼을 경우
        GET_SINGLE(ChatManager)->AddMessage(L"인벤토리가 가득 찼습니다.");
        return false;
    }
}

bool Inventory::AddItem(int ItemId, int ItemCount)
{
    if (ItemId <= 0)
        return false;

    bool found = false;
    int emptySlot = 0;
    const auto itemType = GET_SINGLE(ItemManager)->GetItem(ItemId).Type;
    const auto itemMaxCount = GET_SINGLE(ItemManager)->GetItem(ItemId).MaxCount;

    // 빈 슬롯 개수 확인
    for (auto& slot : _slots)
    {
        if (slot == nullptr)
            return false;

        if (slot->ItemId == 0)
            emptySlot++;

        if (slot->ItemId == ItemId)
            found = true;
    }

    for (auto& slot : _slots)
    {
        // 아이템이 이미 존재하는 경우
        if (found)
        {
            int index = 0;
            // 장비류는 슬롯을 따로 차지함
            if (itemType == L"Wearable")
            {
                if (emptySlot < ItemCount)
                {
                    // 추가하려는 양이 빈 슬롯보다 더 많음
                    return false;
                }

                int addItemCount = 0;

                // 빈 슬롯을 찾아 아이템 추가
                for (auto& slot : _slots)
                {
                    if (slot == nullptr)
                        return false;

                    if (slot->ItemId == 0)
                    {
                        slot->index = index;
                        addItemCount++;
                        slot->ItemId = ItemId;
                        slot->ItemCount = 1;
                        AddItemToServer(ItemId, 1);
                        SetItemSlot(*slot.get());

                        if (addItemCount == ItemCount)
                            return true;
                    }
                    index++;
                }
            }
            else if (itemType == L"Consumable")
            {
                // 해당 슬롯을 찾아 수량 증가
                for (auto& slot : _slots)
                {
                    if (slot->ItemId == ItemId)
                    {
                        // 추가한 양이 최대 개수보다 적을 때
                        if (slot->ItemCount + ItemCount <= itemMaxCount)
                        {
                            slot->index = index;
                            // 수량 증가
                            slot->ItemCount += ItemCount;
                            AddUseableItemToServer(ItemId, ItemCount);
                            SetItemSlot(*slot.get());
                            return true;
                        }
                    }
                    index++;
                }
            }
            else
            {
                // 해당 슬롯을 찾아 수량 증가
                for (auto& slot : _slots)
                {
                    if (slot->ItemId == ItemId)
                    {
                        // 추가한 양이 최대 개수보다 적을 때
                        if (slot->ItemCount + ItemCount <= itemMaxCount)
                        {
                            slot->index = index;
                            // 수량 증가
                            slot->ItemCount += ItemCount;
                            AddItemToServer(ItemId, ItemCount);
                            SetItemSlot(*slot.get());
                            return true;
                        }
                    }
                    index++;
                }
            }
        }
        // 아이템이 존재하지 않는 경우
        else if (found == false)
        {
            int index = 0;

            // 장비류는 슬롯을 따로 차지함
            if (itemType == L"Wearable")
            {
                if (emptySlot < ItemCount)
                {
                    // 추가하려는 양이 빈 슬롯보다 더 많음
                    return false;
                }

                int addItemCount = 0;

                // 빈 슬롯을 찾아 아이템 추가
                for (auto& slot : _slots)
                {
                    if (slot == nullptr)
                        continue;

                    if (slot->ItemId == 0)
                    {
                        slot->index = index;
                        addItemCount++;
                        slot->ItemId = ItemId;
                        slot->ItemCount = 1;
                        AddItemToServer(ItemId, 1);
                        SetItemSlot(*slot.get());

                        if (addItemCount == ItemCount)
                            return true;
                    }
                    index++;
                }
                
                return true;
            }
            else if (itemType == L"Consumable")
            {
                if (emptySlot < 1)
                {
                    // 추가하려는 양이 빈 슬롯보다 더 많음
                    return false;
                }

                // 빈 슬롯을 찾아 아이템 추가
                for (auto& slot : _slots)
                {
                    if (slot == nullptr)
                        break;

                    if (slot->ItemId == 0)
                    {
                        // 추가한 양이 최대 개수보다 적을 때
                        if (slot->ItemCount + ItemCount <= itemMaxCount)
                        {
                            slot = make_shared<ITEM>();
                            slot->index = index;
                            slot->ItemId = ItemId;
                            slot->ItemCount += ItemCount;
                            AddUseableItemToServer(ItemId, ItemCount);
                            SetItemSlot(*slot.get());
                            return true;
                        }
                    }
                    index++;
                }
            }
            else
            {
                if (emptySlot < 1)
                {
                    // 추가하려는 양이 빈 슬롯보다 더 많음
                    return false;
                }

                // 빈 슬롯을 찾아 아이템 추가
                for (auto& slot : _slots)
                {
                    if (slot == nullptr)
                        break;

                    if (slot->ItemId == 0)
                    {
                        // 추가한 양이 최대 개수보다 적을 때
                        if (slot->ItemCount + ItemCount <= itemMaxCount)
                        {
                            slot->index = index;
                            slot->ItemId = ItemId;
                            slot->ItemCount += ItemCount;
                            AddItemToServer(ItemId, ItemCount);
                            SetItemSlot(*slot.get());
                            return true;
                        }
                    }
                    index++;
                }
            }
        }
    }
    return false;
}

bool Inventory::RemoveItem(shared_ptr<ITEM> item)
{
    // 기본 무기는 제거 불가
    int itemId = item->ItemId;
    if (itemId == 1 || itemId == 2 || itemId == 3)
        return false;

    int index = 0;

    for (auto& slot : _slots)
    {
        if (slot == item)
        {
            slot->index = index;
            slot->ItemCount--;

            AddUseableItemToServer(item->ItemId, -1);

            AddItemToServer(itemId, -1);

            if (slot->ItemCount <= 0)
            {
                /*RECT slotRect = slot->Rect;*/
                slot->Reset();
                /*slot->Rect = slotRect;*/
                return true;
            }
        }
        index++;
    }
    return false;
}

bool Inventory::RemoveItem(shared_ptr<ITEM> item, int ItemCount)
{
    if (item == nullptr)
        return false;

    // 기본 무기는 제거 불가
    int itemId = item->ItemId;
    if (itemId == 1 || itemId == 2 || itemId == 3)
        return false;

    int index = 0;

    for (auto& slot : _slots)
    {
        if (slot == item)
        {
            slot->index = index;
            slot->ItemCount = 0;

            AddUseableItemToServer(item->ItemId, 0);
            AddItemToServer(itemId, -ItemCount);

            if (slot->ItemCount <= 0)
            {
                //RECT slotRect = slot->Rect;
                slot->Reset();
                //slot->Rect = slotRect;
                return true;
            }
        }
        index++;
    }
    return false;
}

bool Inventory::RemoveItem(int itemId)
{
    bool found = false;
    // 기본 무기는 제거 불가
    if (itemId == 1 || itemId == 2 || itemId == 3)
        return false;
    
    int index = 0;

    // 아이템이 이미 존재하는 경우
    for (auto& slot : _slots)
    {
        if (slot->ItemId == itemId)
        {
            found = true;
        }

        if (found == true)
        {
            slot->index = index;
            slot->ItemCount--; // 수량 감소

            AddUseableItemToServer(itemId, -1);
            AddItemToServer(itemId, -1);

            if (slot->ItemCount <= 0)
            {
                //RECT slotRect = slot->Rect;
                slot->Reset();
                //slot->Rect = slotRect;
                return true;
            }
        }
        index++;
    }

    // 아이템이 없는 경우
    return false;
}

bool Inventory::RemoveItem(int itemId, int ItemCount)
{
    bool found = false;
    // 기본 무기는 제거 불가
    if (itemId == 1 || itemId == 2 || itemId == 3)
        return false;

    int index = 0;

    // 아이템이 이미 존재하는 경우
    for (auto& slot : _slots)
    {
        if (slot->ItemId == itemId)
        {
            found = true;
        }

        if (found == true)
        {
            // 제거하려는 아이템의 갯수가 지정된 양보다 적을 경우
            if (slot->ItemCount < ItemCount)
                return false;

            slot->ItemCount -= ItemCount; // 수량 감소

            AddUseableItemToServer(itemId, -ItemCount);
            AddItemToServer(itemId, -ItemCount);

            if (slot->ItemCount <= 0)
            {
                RECT slotRect = slot->Rect;
                slot->Reset();
                slot->Rect = slotRect;
                return true;
            }
        }
    }

    // 아이템이 없는 경우
    return false;
}

void Inventory::SetItemCount(int itemId, int ItemCount)
{
    if (itemId <= 0)
        return;

    shared_ptr<ITEM> item = FindItemFromInventory(itemId);
    if (item == nullptr)
    {
        if (AddItem(itemId, 1))
        {
            shared_ptr<ITEM> addedItem = FindItemFromInventory(itemId);
            addedItem->ItemCount = ItemCount;
            return;
        }
    }
    else
    {
        item->ItemCount = ItemCount;
    }
}

void Inventory::ChangeItem(ITEM& itemFrom, ITEM& itemTo)
{
    ITEM temp = itemFrom;
    RECT fromRect = itemFrom.Rect;
    RECT toRect = itemTo.Rect;

    itemFrom = itemTo;
    itemFrom.Rect = fromRect;
    itemTo = temp;
    itemTo.Rect = toRect;
}

shared_ptr<ITEM> Inventory::FindItemFromInventory(int itemId)
{
    for (auto& slot : _slots)
    {
        if (slot == nullptr)
            continue;

        if (slot->ItemId == itemId)
            return slot;
    }

    return nullptr;
}

int Inventory::FindItemIndexFromInventory(int itemId)
{
    int index = 0;

    for (auto& slot : _slots)
    {
        if (slot->ItemId == itemId)
            return index;

        index++;
    }
    return -1;
}

shared_ptr<ITEM> Inventory::FindItemFromInventory(shared_ptr<ITEM> item)
{
    for (auto& slot : _slots)
    {
        if (slot == item)
            return slot;
    }
    return nullptr;
}

ITEM Inventory::GetEquippedItem(wstring wstr)
{
    if (wstr == L"Weapon")
        return _equips[0].second;

    if (wstr == L"Helmet")
        return _equips[1].second;

    if (wstr == L"Armor")
        return _equips[2].second;

    if (wstr == L"Pants")
        return _equips[3].second;

    if (wstr == L"Boots")
        return _equips[4].second;

    return {};
}

void Inventory::EquipItem(shared_ptr<ITEM> item)
{
    if (item == nullptr)
        return;

    if (item->Type != L"Wearable")
        return;

    auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
    if (myPlayer == nullptr)
        return;

    ITEM newItem = GET_SINGLE(ItemManager)->GetItem(item->ItemId);

    if (item->SubType == L"Sword")
    {
        if (_equips[0].second.ItemId == item->ItemId)
            return;

        _equips[0].second = newItem;
        myPlayer->SetWeaponType(Protocol::WEAPON_TYPE_SWORD);
    }
    else if (item->SubType == L"Bow")
    {
        if (_equips[0].second.ItemId == item->ItemId)
            return;

        _equips[0].second = newItem;
        myPlayer->SetWeaponType(Protocol::WEAPON_TYPE_BOW);
    }
    else if (item->SubType == L"Staff")
    {
        if (_equips[0].second.ItemId == item->ItemId)
            return;

        _equips[0].second = newItem;
        myPlayer->SetWeaponType(Protocol::WEAPON_TYPE_STAFF);
    }
    else if (item->SubType == L"Helmet")
    {
        if (_equips[1].second.ItemId == item->ItemId)
            return;

        // 장착되어 있지 않은 경우
        if (_equips[1].second.ItemId == 0)
        {
            _equips[1].second = newItem;
            SyncEquips(newItem.ItemId);
            RemoveItem(item);
        }
        else
        {   // 이미 장착되어 있는 경우
            ITEM temp = _equips[1].second;
            _equips[1].second = newItem;
            AddItem(temp.ItemId);
            SyncEquips(newItem.ItemId);
            RemoveItem(item);
        }
    }
    else if (item->SubType == L"Armor")
    {
        if (_equips[2].second.ItemId == item->ItemId)
            return;

        // 장착되어 있지 않은 경우
        if (_equips[2].second.ItemId == 0)
        {
            _equips[2].second = newItem;
            SyncEquips(newItem.ItemId);
            RemoveItem(item);
        }
        else
        {   // 이미 장착되어 있는 경우
            ITEM temp = _equips[2].second;
            _equips[2].second = newItem;
            AddItem(temp.ItemId);
            SyncEquips(newItem.ItemId);
            RemoveItem(item);
        }
    }
    else if (item->SubType == L"Pants")
    {        // 장착되어 있지 않은 경우
        if (_equips[3].second.ItemId == 0)
        {
            _equips[3].second = newItem;
            SyncEquips(newItem.ItemId);
            RemoveItem(item);
        }
        else
        {   // 이미 장착되어 있는 경우
            ITEM temp = _equips[3].second;
            _equips[3].second = newItem;
            AddItem(temp.ItemId);
            SyncEquips(newItem.ItemId);
            RemoveItem(item);
        }
    }
    else if (item->SubType == L"Boots")
    {        // 장착되어 있지 않은 경우
        if (_equips[4].second.ItemId == 0)
        {
            _equips[4].second = newItem;
            SyncEquips(newItem.ItemId);
            RemoveItem(item);
        }
        else
        {   // 이미 장착되어 있는 경우
            ITEM temp = _equips[4].second;
            _equips[4].second = newItem;
            AddItem(temp.ItemId);
            SyncEquips(newItem.ItemId);
            RemoveItem(item);
        }
    }

    int itemAttack = 0;
    int itemDefence = 0;

    for (const auto& item : _equips)
    {
        if (item.second.SubType == L"Pants")
        {
            item.second.PotionEffect;
            item.second.PotionMaxCount;
        }

        itemAttack += item.second.Attack;
        itemDefence += item.second.Defence;
    }

    // 기본 공격력 + 아이템 공격력
    myPlayer->info.set_attack(10 + itemAttack);
    myPlayer->info.set_defence(itemDefence);

}

void Inventory::QuickEquipItem(int itemID)
{
    if (itemID <= 0)
        return;

    ITEM item = GET_SINGLE(ItemManager)->GetItem(itemID);

    if (item.Type != L"Wearable" && item.Type != L"Consumable")
        return;

    auto myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
    if (myPlayer == nullptr)
        return;

    if (item.SubType == L"Sword")
    {
        if (_equips[0].second.ItemId == item.ItemId)
            return;

        _equips[0].second = item;
        myPlayer->SetWeaponType(Protocol::WEAPON_TYPE_SWORD);
    }
    else if (item.SubType == L"Bow")
    {
        if (_equips[0].second.ItemId == item.ItemId)
            return;

        _equips[0].second = item;
        myPlayer->SetWeaponType(Protocol::WEAPON_TYPE_BOW);
    }
    else if (item.SubType == L"Staff")
    {
        if (_equips[0].second.ItemId == item.ItemId)
            return;

        _equips[0].second = item;
        myPlayer->SetWeaponType(Protocol::WEAPON_TYPE_STAFF);
    }
    else if (item.SubType == L"Potion")
    {


    }
}

void Inventory::PressToSetQuickItem(ITEM slot)
{
    auto slotRef = make_shared<ITEM>(slot);

    // 퀵 슬롯 등록
    if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_1))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(slotRef, 1);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_2))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(slotRef, 2);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_3))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(slotRef, 3);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_4))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(slotRef, 4);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_5))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(slotRef, 5);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_6))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(slotRef, 6);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_7))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(slotRef, 7);
    }
    else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::KEY_8))
    {
        GET_SINGLE(ItemManager)->SetItemToQuickSlot(slotRef, 8);
    }
}

void Inventory::OnPopClickAcceptDelegate()
{
    if (RemoveItem(_deleteItem, _deleteItem->ItemCount))
        _deleteItem = nullptr;
}

void Inventory::ResetInventory()
{
    // 부활 이후 인벤토리의 모든 정보를 서버에서 가져옴
    for (auto& slot : _slots)
    {
        auto tempRect = slot->Rect;
        slot->Reset();
        slot->Rect = tempRect;
    }

    for (auto& slot : _equips)
    {
        auto tempRect = slot.first;
        slot.second.Reset();
        slot.first = tempRect;
    }

    // 서버와 동기화

}
