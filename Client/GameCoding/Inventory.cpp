#include "pch.h"
#include "Inventory.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "FileManager.h"
#include "ChatManager.h"
#include "TextBox.h"

Inventory::Inventory()
{
	_background = GET_SINGLE(ResourceManager)->GetSprite(L"Inventory");
    _itemSprite = GET_SINGLE(ResourceManager)->GetSprite(L"Sword");

    { // ������ ����
        wstring wstr = L"������ ����\nPlace Holder";
        _textBox = new TextBox(wstr);
        _textBox->SetPos({ 600, 350 });
        _textBox->SetSize({ 160, 25 });
        AddChild(_textBox);
    }
}

Inventory::~Inventory()
{

}

void Inventory::BeginPlay()
{
	_itemTable = GET_SINGLE(FileManager)->GetDataFromCSV("E:\\Cpp\\IOCP\\Server\\Client\\Resources\\Table\\ItemTable.csv");

    _slots.push_back({ 1, 1 });
    _slots.push_back({ 2, 1 });
    _slots.push_back({ 3, 1 });


}

void Inventory::Tick()
{
    int i = 0;

    int _initialX = _pos.x + 6;
    int _initialY = _pos.y + 42;

    // �������� Sprite�� ã�� ����
    for (auto& slot : _slots)
    {
        int j = i / 7;

        if (slot.ItemId == 0)
            return;

        if (slot.Sprite == nullptr)
        {
            // �̸��� Sprite, Rect, Description, Type �Ҵ�
            vector<wstring> ItemInfo = findItemInfo(slot.ItemId);

            slot.Sprite = GET_SINGLE(ResourceManager)->GetSprite(slot.Name);
            slot.Rect = { _initialX + (34 * i), _initialY + (34 * j), _initialX + (34 * i) + _slotSize,  _initialY + (34 * j) + _slotSize };
            slot.Name = GetName(ItemInfo);
            slot.Description = GetDescription(ItemInfo);
            slot.Type = GetType(ItemInfo);
        }
        else
        {
            i++;
            continue;
        }

        i++;
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

    for (int i = 0; i < _slots.size(); i++)
    {
        int j = i / 7;

        int _initialX = _pos.x + 6;
        int _initialY = _pos.y + 42;

        if (_slots[i].Sprite == nullptr)
            continue;

        ::TransparentBlt(hdc,
            _initialX + (34 * i),
            _initialY + (34 * j),
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

    for (auto& slot : _slots)
    {
        if (IsMouseInRect(slot.Rect))
        {
            _textBox->ChangeText(slot.Description);
        }
    }

    for (auto& child : _children)
        child->Render(hdc);
}

void Inventory::AddItem(int ItemId)
{
    bool found = false;

    // _slots ��ȸ
        for (auto& slot : _slots)
        {
            // �������� �̹� �����ϴ� ���
            if (slot.ItemId == ItemId)
            {
                slot.Count++; // ���� ����
                found = true;
                break;
            }
        }

    // �������� �������� �ʴ� ���
    if (found == false)
    {
        // ���� �տ� ������ �߰�
        for (auto& slot : _slots)
        {
            // �������� ���� ���
            if (slot.ItemId == 0)
            {
                slot.ItemId = ItemId; // ������ ID ����
                slot.Count++;
                return;
            }
        }
    }
}

void Inventory::RemoveItem(int ItemId)
{
    bool found = false;

    // _slots ��ȸ
    for (auto& slot : _slots)
    {
        // �������� �̹� �����ϴ� ���
        if (slot.ItemId == ItemId)
        {
            slot.Count--; // ���� ����
            found = true;

            if (slot.Count <= 0)
            {
                slot.ItemId = 0;
                slot.Count = 0;
            }
            break;
        }
    }

    // �������� �������� �ʴ� ���
    if (found == false)
    {
        return;
    }
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

wstring Inventory::GetType(vector<wstring> row)
{
    return row[2];
}

wstring Inventory::GetDescription(vector<wstring> row)
{
    return row[3];
}