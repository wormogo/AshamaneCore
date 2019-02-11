/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eTirisfalGlades
{
    SPELL_RAISE_UNDEAD           = 93446,
    SPELL_SUMMON_RISEN_DEAD      = 93447,
    SPELL_DIRT_EXPLOSION         = 89199,
    SPELL_CHURNING_DIRT          = 92788,
    SPELL_ENRAGE                 = 44427,
    SPELL_RISEN_DEAD_TRANSFORM   = 93460,

    RISEN_DEAD_PATH_01           = 5037400,
    RISEN_DEAD_PATH_02           = 5037401,

    NPC_ARADNE                   = 50372,
    NPC_RISEN_RECRUIT            = 50414,
    NPC_DEATHKNELL_GRAVE_TARGET  = 50373,

    RISEN_RECRUIT_PATH_01        = 5041400,
    RISEN_RECRUIT_PATH_02        = 5041401,

    QUEST_THE_SHADOW_GRAVE       = 28608,
    SPELL_SUMMON_DARNELL         = 91576,
};

class npc_aradne : public CreatureScript
{
public:
    npc_aradne() : CreatureScript("npc_aradne") { }

    struct npc_aradneAI : public ScriptedAI
    {
        npc_aradneAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;

        void Reset()
        {
            m_timer = 0;
            m_phase = 0;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == WAYPOINT_MOTION_TYPE)
            switch (id)
            {
                case 4:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 12:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 15:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 20:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 26:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 31:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 33:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 37:
                    m_phase = 1;
                    m_timer = 100;
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            switch (m_phase)
            {
                case 1:
                    if (Creature* npc = me->FindNearestCreature(NPC_DEATHKNELL_GRAVE_TARGET, 15.0f))
                        me->SetFacingToObject(npc);

                    m_timer = 500;
                    m_phase = 2;
                    break;
                case 2:
                    if (Creature* npc = me->FindNearestCreature(NPC_DEATHKNELL_GRAVE_TARGET, 15.0f))
                        me->CastSpell(npc, SPELL_RAISE_UNDEAD, false);

                    m_timer = 2000;
                    m_phase = 3;
                    break;
                case 3:
                    Talk(0);
                    m_timer = 500;
                    m_phase = 4;
                    break;
                default:
                    m_timer = 0;
                    m_phase = 0;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_aradneAI(pCreature);
    }
};

class npc_deathknell_grave_target : public CreatureScript
{
public:
    npc_deathknell_grave_target() : CreatureScript("npc_deathknell_grave_target") { }

    struct npc_deathknell_grave_targetAI : public ScriptedAI
    {
        npc_deathknell_grave_targetAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;

        void Reset()
        {
            m_timer = 0;
            m_phase = 0;
        }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_RAISE_UNDEAD)
            {
                m_timer = 100;
                m_phase = 1;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            switch (m_phase)
            {
                case 1:
                    DoCast(me, SPELL_DIRT_EXPLOSION);
                    DoCast(me, SPELL_CHURNING_DIRT);
                    m_phase = 2;
                    break;
                case 2:
                    me->CastSpell(me, SPELL_SUMMON_RISEN_DEAD, true);
                    m_phase = 3;
                    m_timer = 3000;
                    break;
                case 3:
                    me->RemoveAllAuras();
                    m_phase = 4;
                    break;
                case 4:
                    m_timer = 0;
                    m_phase = 0;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_deathknell_grave_targetAI(pCreature);
    }
};

class npc_risen_dead : public CreatureScript
{
public:
    npc_risen_dead() : CreatureScript("npc_risen_dead") { }

    struct npc_risen_deadAI : public ScriptedAI
    {
        npc_risen_deadAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;
        uint32 m_WishToBeDead;

        void Reset()
        {
            m_timer = 2000;
            m_phase = 0;
            m_WishToBeDead = urand(0, 100);
        }

        void UpdateAI(const uint32 diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            if (m_WishToBeDead < 33)
                ShowDead();
            else if (m_WishToBeDead < 66)
                ShowLife();
            else
                ShowRunning();
        }

        void ShowDead()
        {
            switch (m_phase)
            {
                case 0:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_QUESTION);
                    m_timer = 200;
                    m_phase = 1;
                    break;
                case 1:
                    if (Creature* aradne = me->FindNearestCreature(NPC_ARADNE, 20.0f))
                        Talk(7, aradne);
                    m_timer = 5000;
                    m_phase = 2;
                    break;
                case 2:
                    Talk(2);
                    m_timer = 5000;
                    m_phase = 3;
                    break;
                case 3:
                    Talk(1);
                    me->Kill(me);
                    m_timer = 5000;
                    m_phase = 4;
                    break;
                case 4:
                    m_timer = 0;
                    m_phase = 5;
                    break;
            }
        }

        void ShowRunning()
        {
            switch (m_phase)
            {
                case 0:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_QUESTION);
                    m_timer = 200;
                    m_phase = 1;
                    break;
                case 1:
                    Talk(0);
                    m_timer = 5000;
                    m_phase = 2;
                    break;
                case 2:
                    Talk(3);
                    m_timer = 5000;
                    m_phase = 3;
                    break;
                case 3:
                    Talk(6);
                    me->CastSpell(me, SPELL_ENRAGE, true);
                    Talk(8);
                    me->GetMotionMaster()->MovePath(RISEN_DEAD_PATH_01, false);
                    m_timer = 5000;
                    m_phase = 4;
                    break;
                case 4:
                    m_timer = 0;
                    m_phase = 5;
                    break;
            }
        }

        void ShowLife()
        {
            switch (m_phase)
            {
                case 0:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_QUESTION);
                    m_timer = 200;
                    m_phase = 1;
                    break;
                case 1:
                    Talk(0);
                    m_timer = 5000;
                    m_phase = 2;
                    break;
                case 2:
                    Talk(5);
                    m_timer = 5000;
                    m_phase = 3;
                    break;
                case 3:
                    Talk(4);
                    SetEquipmentSlots(false, 10614, 1984);
                    me->GetMotionMaster()->MovePath(RISEN_DEAD_PATH_02, false);
                    m_timer = 20000;
                    m_phase = 4;
                    break;
                case 4:
                    m_timer = 0;
                    m_phase = 5;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_risen_deadAI(pCreature);
    }
};

class npc_undertaker_mordo : public CreatureScript
{
public:
    npc_undertaker_mordo() : CreatureScript("npc_undertaker_mordo") { }

    struct npc_undertaker_mordoAI : public ScriptedAI
    {
        npc_undertaker_mordoAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;
        Creature* m_npc;

        void Reset()
        {
            m_timer = urand(30, 45) * IN_MILLISECONDS;
            m_phase = 0;
            m_npc = nullptr;
        }

        void UpdateAI(const uint32 diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            switch (m_phase)
            {
                case 0:
                    if (Creature* npc_trigger = me->FindNearestCreature(NPC_DEATHKNELL_GRAVE_TARGET, 20.0f))
                        if (m_npc = me->SummonCreature(NPC_RISEN_RECRUIT, npc_trigger->GetPositionX(), npc_trigger->GetPositionY(), npc_trigger->GetPositionZ(), 1.0f, TEMPSUMMON_TIMED_DESPAWN, 46000))
                            m_npc->GetMotionMaster()->MovePath(RISEN_RECRUIT_PATH_01, false);
                    m_phase = 1;
                    break;
                case 1:
                    Talk(0);
                    m_timer = 3000;
                    m_phase = 2;
                    break;
                case 2:
                    if (m_npc && m_npc->IsAlive())
                    {
                        m_npc->SetUInt32Value(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_KNEEL);
                        m_npc->SetFacingToObject(me);
                    }
                    m_timer = 750;
                    m_phase = 3;
                    break;
                case 3:
                    me->EmoteWithDelay(6000, EMOTE_STATE_USE_STANDING);
                    m_timer = 7000;
                    m_phase = 4;
                    break;
                case 4:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
                    m_timer = 2500;
                    m_phase = 5;
                    break;
                case 5:
                    Talk(1);
                    m_timer = 250;
                    m_phase = 6;
                    break;
                case 6:
                    if (m_npc && m_npc->IsAlive())
                    {
                        m_npc->AddAura(SPELL_RISEN_DEAD_TRANSFORM, m_npc);
                        m_npc->CastSpell(m_npc, SPELL_RISEN_DEAD_TRANSFORM);
                        m_npc->SetDisplayId(36775);
                        m_npc->SetUInt32Value(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_STAND);
                    }
                    m_timer = 2500;
                    m_phase = 7;
                    break;
                case 7:
                    if (m_npc && m_npc->IsAlive())
                    {
                        m_npc->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                        m_npc->AI()->Talk(0);
                    }
                    m_timer = 3750;
                    m_phase = 8;
                    break;
                case 8:
                    if (m_npc && m_npc->IsAlive())
                        m_npc->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                    m_timer = 2500;
                    m_phase = 9;
                    break;
                case 9:
                    if (m_npc && m_npc->IsAlive())
                        m_npc->GetMotionMaster()->MovePath(RISEN_RECRUIT_PATH_02, false);
                    m_timer = urand(15, 45) * IN_MILLISECONDS;
                    m_phase = 0;
                    break;
            }
        }

        void sQuestAccept(Player* player, Quest const* quest) override
        {
            if (quest->GetQuestId() == QUEST_THE_SHADOW_GRAVE)
                player->CastSpell(player, SPELL_SUMMON_DARNELL);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_undertaker_mordoAI(pCreature);
    }
};

class npc_mindless_zombie : public CreatureScript
{
public:
    npc_mindless_zombie() : CreatureScript("npc_mindless_zombie") { }

    struct npc_mindless_zombieAI : public ScriptedAI
    {
        npc_mindless_zombieAI(Creature *c) : ScriptedAI(c) { }

        EventMap events;
        bool _crazed;

        void Reset()
        {
            me->RemoveAllAuras();
            _crazed = false;

            if (!_crazed)
            {
                _crazed = true;
                events.ScheduleEvent(1, 28s, 57s);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            if (!me->IsInCombat())
            {
                if (Player* player = me->SelectNearestPlayer(15.0f))
                {
                    if (!player->IsInCombat())
                    {
                        while (uint32 eventId = events.ExecuteEvent())
                        {
                            switch (eventId)
                            {
                                case 1:
                                {
                                    me->SetFacingToObject(player);
                                    me->CastSpell(me, SPELL_ENRAGE, true);
                                    me->SetWalk(false);
                                    me->GetMotionMaster()->MoveFollow(player, 1.0f, 0.0f);
                                    events.ScheduleEvent(2, 4s, 6s);
                                    break;
                                }
                                case 2:
                                {
                                    me->GetMotionMaster()->Clear();
                                    me->RemoveAllAuras();
                                    events.ScheduleEvent(1, 28s, 57s);
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mindless_zombieAI(pCreature);
    }
};

void AddSC_tirisfal_glades()
{
    new npc_aradne();
    new npc_deathknell_grave_target();
    new npc_risen_dead();
    new npc_undertaker_mordo();
    new npc_mindless_zombie();
}
