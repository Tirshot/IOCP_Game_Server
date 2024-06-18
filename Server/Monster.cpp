#include "pch.h"
#include "Monster.h"
#include "GameRoom.h"
#include "Player.h"
#include "Item.h"
#include "Chat.h"
#include "GameSession.h"
#include "GameObject.h"
#include "ItemManager.h"

Monster::Monster()
{
	info.set_objecttype(Protocol::OBJECT_TYPE_MONSTER);
	info.set_monstertype(Protocol::MONSTER_TYPE_SNAKE);
	info.set_hp(50);
	info.set_maxhp(50);
	info.set_attack(1);
	info.set_defence(0);
}

Monster::~Monster()
{
}

void Monster::Init()
{

}

void Monster::Update()
{
	switch (info.state())
	{
	case IDLE:
		UpdateIdle();
		break;

	case MOVE:
		UpdateMove();
		break;

	case SKILL:
		UpdateSkill();
		break;

	case HIT:
		UpdateHit();
		break;
	}
}

void Monster::UpdateIdle()
{
	// GameObject�� ��� �ִ� ���� ���� üũ
	if (room == nullptr)
		return;

	// �ֺ��� �ִ� �÷��̾� ã��
	if (_target.lock() == nullptr)
		_target = room->FindClosestPlayer(GetCellPos());

	PlayerRef target = _target.lock();

	if (target)
	{
		// ���� �Ǵ�
		Vec2Int dir = target->GetCellPos() - GetCellPos();
		int32 dist = abs(dir.x) + abs(dir.y);
		if (dist == 1)
		{
			// �����ϱ�
			SetDir(GetLookAtDir(target->GetCellPos()));
			target->OnDamaged(shared_from_this());
			{
				int32 damage = info.attack() - target->info.defence();
				SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Hit(target->info.objectid(), info.objectid(), damage);
				target->session->Send(sendBuffer);
			}

			// �ֺ��� Ŭ���̾�Ʈ�� �˸�
			SetState(SKILL, true);
			_waitUntil = GetTickCount64() + 1000; // 1�� ��ٸ�
		}
		else
		{
			// ��ǥ ã��
			vector<Vec2Int> path;
			if (room->FindPath(GetCellPos(), target->GetCellPos(), OUT path))
			{
				if (path.size() > 1)
				{
					// �� ĭ�� �̵�
					Vec2Int nextPos = path[1];
					if (room->MonsterCanGo(nextPos))
					{
						SetDir(GetLookAtDir(nextPos));
						SetCellPos(nextPos);
						_waitUntil = GetTickCount64() + 1000; // 1�� ��ٸ�

						// ������ Ŭ���̾�Ʈ�� �˸�
						SetState(MOVE, true);
					}
				}
				else
					// ���� �� ã������ ����
				{
					SetCellPos(path[0]);
				}
			}
			// �ʹ� �־ ���� ã�� ����
			else
			{
				_target.reset();
			}
		}
	}
}

void Monster::UpdateMove()
{
	uint64 now = GetTickCount64();

	if (_waitUntil > now)
		return;

	SetState(IDLE);
}

void Monster::UpdateSkill()
{
	uint64 now = GetTickCount64();

	if (_waitUntil > now)
		return;

	SetState(IDLE);
}

void Monster::UpdateHit()
{
	uint64 now = GetTickCount64();

	if (_waitHit > now)
		return;

	SetState(IDLE);
}

void Monster::ItemDrop(CreatureRef owner)
{
	// ���� Ÿ�Կ� ���� ������ ��� �׷� ���̺� ��������
	auto monsterType = info.monstertype();
	auto groupInfo = GET_SINGLE(ItemManager)->GetMonsterDropItemGroupInfo(monsterType);

	if (groupInfo.empty())
		return;

	// �׷� ���̺��� ������ �׷� �� Ȯ�� ��������
	int WearableProbablity = GET_SINGLE(ItemManager)->GetWearableProbablity(groupInfo);
	int ConsumableProbablity = WearableProbablity + GET_SINGLE(ItemManager)->GetConsumableProbablity(groupInfo);
	int ETCProbablity = ConsumableProbablity + GET_SINGLE(ItemManager)->GetETCProbablity(groupInfo);
	int NoneProbablity = ETCProbablity + GET_SINGLE(ItemManager)->GetNoneProbablity(groupInfo);

	// �ֻ��� ������
	int randValue = rand() % 100 + 1; // 1 ~ 100

	// �ش� ���� Ÿ�Կ� ��ȿ�� ���� ������ ��� ���̺� ��������
	auto dropTable = GET_SINGLE(ItemManager)->GetItemDropTableByMonsterType(monsterType);

	vector<vector<wstring>> group;

	// �׷� Ȯ���� ���� ������ ���
	if (randValue <= WearableProbablity)
	{
		// ��� ������ ���
		group = GET_SINGLE(ItemManager)->GetItemDropTableByGroup(dropTable, L"Wearable");
	}
	else if (randValue > WearableProbablity && randValue <= ConsumableProbablity)
	{
		// �Ҹ�ǰ ������ ���
		group = GET_SINGLE(ItemManager)->GetItemDropTableByGroup(dropTable, L"Consumable");
	}
	else if (randValue > ConsumableProbablity && randValue <= ETCProbablity)
	{
		// ��Ÿ ������ ��� - ����Ʈ ������ ��
		group = GET_SINGLE(ItemManager)->GetItemDropTableByGroup(dropTable, L"ETC");
	}
	else if (randValue > ETCProbablity && randValue <= NoneProbablity)
	{
		// �ƹ� �����۵� ������� ����
		return;
	}

	// ������ ����, ��Ŷ ����
	GET_SINGLE(ItemManager)->MakeItem(group, owner, {GetCellPos().x, GetCellPos().y});
}

void Monster::GoldDrop(CreatureRef owner)
{
	// �׷� ���̺��� �ּ� ���, �ִ� ��带 ����
	auto itemGroupinfo = GET_SINGLE(ItemManager)->GetMonsterDropItemGroupInfo(info.monstertype());
	int minGold = GET_SINGLE(ItemManager)->GetMinGold(itemGroupinfo);
	int maxGold = GET_SINGLE(ItemManager)->GetMaxGold(itemGroupinfo);

	// �ִ�� �ּ��� ���� ���� �� ���� ��밪�� �ִ��� ����
	int step = maxGold - minGold;

	// �ּ� ~ �ִ� ������ ���� ����
	int randValue = rand() % step + 1;

	// ��� ���� �� ��Ŷ ����
	SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Gold(owner->GetObjectID(), randValue);
	PlayerRef player = static_pointer_cast<Player>(GRoom->FindObject(owner->GetObjectID()));

	if (player)
	{
		player->session->Send(sendBuffer);
		GChat->AddText(format(L"Player{0}, {1} gold ȹ��.", owner->GetObjectID(), randValue));
	}
}

void Monster::OnDamaged(CreatureRef attacker, bool debug)
{
	Super::OnDamaged(attacker);

	// �� ���� ����� Ŀ�ǵ�� ���Ͱ� óġ������ ��
	if (debug)
	{
		// ��� ó��
		if (room)
		{
			room->RemoveObject(GetObjectID());

			PlayerRef player = dynamic_pointer_cast<Player>(attacker);
			if (player)
				player->QuestProgress(0);

			// ä�� ���
			GChat->AddText(format(L"debug : {0} {1}��(��) {2}�� ���� óġ��.",
				GetName(),
				GetObjectID(),
				attacker->GetName()));

			// ������ ���
			ItemDrop(attacker);
			GoldDrop(attacker);

			return;
		}
	}

	if (info.hp() == 0)
	{
		// ��� ó��
		if (room)
		{
			room->RemoveObject(GetObjectID());

			PlayerRef player = dynamic_pointer_cast<Player>(attacker);
			if (player)
				player->QuestProgress(0);

			// ä�� ���
			GChat->AddText(format(L"{0} {1}��(��) {2}�� ���� óġ��.",
				GetName(),
				GetObjectID(),
				attacker->GetName()));

			// ������ ���
			ItemDrop(attacker);
			GoldDrop(attacker);
		}
	}
}

wstring Monster::GetName()
{
	return GET_SINGLE(ItemManager)->StringToWString(info.name());
}
