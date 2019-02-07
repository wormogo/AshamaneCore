#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "AreaTriggerTemplate.h"
#include "AreaTriggerAI.h"
#include "the_arcway.h"

enum Spells
{
    SPELL_CHRONO_SHARDS         = 203881,
    SPELL_SUMMON_CHRONO_SHARD   = 203254,
    SPELL_CHRONO_SHARD_PULSE    = 203835,
    SPELL_CHRONO_SHARD_SPAWN    = 203823,
    SPELL_ACCELERATING_BLAST    = 203176,
    SPELL_FORCE_BOMB            = 202974,
    SPELL_FORCE_BOMB_AREA       = 202975,
    SPELL_FORCE_DETONATION      = 203087,
    SPELL_FORCE_NOVA_AREA       = 203090,
    SPELL_FORCE_NOVA_DMG        = 203139,
    SPELL_TIME_SPLIT            = 203833,
    SPELL_BANISH_IN_TIME        = 203882,
    SPELL_BANISH_IN_TIME_AURA   = 203914, // Visual Effect on Screen
    SPELL_BANISH_IN_TIME_TELE   = 203883,
    SPELL_LOST_IN_TIME          = 203935,
    SPELL_UNSTABLE_MANA         = 220871,
    SPELL_UNSTABLE_MANA_DMG     = 220872,

    SPELL_TIME_LOCK             = 203957,
    SPELL_BREACH_DMG            = 203954,
};

enum Events
{
    EVENT_CHRONO_SHARDS         = 1,
    EVENT_ACCELERATING_BLAST    = 2,
    EVENT_FORCE_BOMB            = 3,
    EVENT_UNSTABLE_MANA         = 4,
    EVENT_BANISH_IN_TIME        = 5,

    EVENT_TIME_LOCK             = 6,
};

enum Says
{
    SAY_INTRO           = 0,
    SAY_INTRO_2         = 1,
    SAY_AGGRO           = 2,
    SAY_CHRONO          = 3,
    SAY_BANISH          = 4,
    SAY_REACH_SUCCESFUL = 5,
    SAY_KILL            = 6,
    SAY_WIPE            = 7,
    SAY_DEATH           = 8,
};

Position VandrosTeleportLocations [] = 
{
    { 3144.530f, 4662.319f, 574.185f, 1.41f }, // Naltira Room
    { 3319.854f, 4522.469f, 570.788f, 1.53f }, // Xakal Room
    { 3155.316f, 5102.332f, 623.203f, 5.22f }, // Ivanyr Room
    { 3147.322f, 4887.054f, 617.710f, 0.20f }, // Corstilax Room
};

uint32 ROOM_POSITION = 0;
constexpr uint32 DATA_LOST_IN_TIME = 1;
constexpr uint32 LOST_IN_TIME_PHASE = 290;

class boss_advisor_vandros : public CreatureScript
{
    public:
        boss_advisor_vandros() : CreatureScript("boss_advisor_vandros")
        {}

        struct boss_advisor_vandros_AI : public BossAI
        {
            explicit boss_advisor_vandros_AI(Creature* creature) : BossAI(creature, DATA_ADVISOR_VANDROS)
            {
                _lostInTime = false;
            }

            uint32 GetData(uint32 id) const override
            {
                if (id == DATA_LOST_IN_TIME)
                    return _reachSuccesful ? 0 : 1;

                return 0;
            }

            void KilledUnit(Unit* victim) override
            {
                if (victim && victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }
            
            void JustReachedHome()
            {
                Talk(SAY_WIPE);
                me->SetReactState(REACT_AGGRESSIVE);
                _JustReachedHome();
            }

            void EnterEvadeMode(EvadeReason why) override
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                me->RemoveAllAreaTriggers();
                CreatureAI::EnterEvadeMode(why);
            }
            
            void JustDied(Unit* /**/) override
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                Talk(SAY_DEATH);
                _JustDied();
            }

            void EnterCombat(Unit* /**/) override
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                Talk(SAY_AGGRO);
                _lostInTime = false;
                _EnterCombat();
                events.ScheduleEvent(EVENT_FORCE_BOMB, Seconds(30));
                events.ScheduleEvent(EVENT_CHRONO_SHARDS, Seconds(10));
                events.ScheduleEvent(EVENT_ACCELERATING_BLAST, Seconds(3));
            }

            void DamageTaken(Unit* /**/, uint32 & damage) override
            {
                if (me->HealthBelowPct(50) && !_lostInTime)
                {
                    _lostInTime = true;
                    _reachSuccesful = false;
                    ROOM_POSITION = urand(0,3);
                    events.DelayEvents(5000);
                    events.ScheduleEvent(EVENT_BANISH_IN_TIME, 100);
                }
            }

            void SpellHit(Unit* /**/, SpellInfo const* spell) override
            {
                if (!spell)
                    return;
                
                if (spell->HasEffect(SPELL_EFFECT_INTERRUPT_CAST) && me->HasAura(SPELL_BANISH_IN_TIME))
                {
                    _reachSuccesful = true;
                    Talk(SAY_REACH_SUCCESFUL);
                    me->CastStop();
                    me->SetReactState(REACT_AGGRESSIVE);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BANISH_IN_TIME_AURA);
                    events.ScheduleEvent(EVENT_UNSTABLE_MANA, Seconds(15));
                }
            }

            void ExecuteEvent(uint32 eventId) override
            {
                switch (eventId)
                {
                    case EVENT_BANISH_IN_TIME:
                    {
                        Talk(SAY_BANISH);
                        me->CastSpell(me, SPELL_BANISH_IN_TIME, false);
                        break;
                    }

                    case EVENT_ACCELERATING_BLAST:
                    {
                        DoCastVictim(SPELL_ACCELERATING_BLAST);
                        events.ScheduleEvent(EVENT_ACCELERATING_BLAST, Seconds(7));
                        break;
                    }

                    case EVENT_CHRONO_SHARDS:
                    {
                        if (roll_chance_i(50))
                            Talk(SAY_CHRONO);

                        for (uint32 i = 3; i > 0; --i)
                        {
                            if (i % 2 == 0)
                                DoCast(me, SPELL_CHRONO_SHARDS, true);
                            else
                                DoCast(me, SPELL_SUMMON_CHRONO_SHARD, true);
                        }

                        events.ScheduleEvent(EVENT_CHRONO_SHARDS, Seconds(12));
                        break;
                    }

                    case EVENT_FORCE_BOMB:
                    {
                        DoCast(me, SPELL_FORCE_BOMB);
                        events.ScheduleEvent(EVENT_FORCE_BOMB, Seconds(30));
                        break;
                    }

                    case EVENT_UNSTABLE_MANA:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                            DoCast(target, SPELL_UNSTABLE_MANA);
                        
                        events.ScheduleEvent(EVENT_UNSTABLE_MANA, Seconds(40));
                        break;
                    }

                    default : break;
                }
            }

            private:
                bool _lostInTime;
                bool _reachSuccesful;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_advisor_vandros_AI(creature);
        }
};

class npc_arc_chrono_shard : public CreatureScript
{
    public:
        npc_arc_chrono_shard() : CreatureScript("npc_arc_chrono_shard")
        {}

        struct npc_arc_chrono_shard_AI : public ScriptedAI
        {
            explicit npc_arc_chrono_shard_AI(Creature* creature) : ScriptedAI(creature)
            {
                me->CastSpell(me, SPELL_CHRONO_SHARD_SPAWN, true);
            }
            
            void IsSummonedBy(Unit* /**/) override
            {
                me->CastSpell(me, SPELL_CHRONO_SHARD_PULSE, true);
                me->GetMotionMaster()->MovePoint(0, me->GetRandomNearPosition(25));
                _timerExplotion = 0;
            }

            void MovementInform(uint32 type, uint32 id) override
            {
                if (type == POINT_MOTION_TYPE && id == 0)
                    me->AddUnitState(UNIT_STATE_ROOT);
            }

            void UpdateAI(uint32 diff) override
            {
                _timerExplotion += diff;

                if (_timerExplotion >= 8 * IN_MILLISECONDS)
                {
                    _timerExplotion = 0;
                    DoCast(me, SPELL_TIME_SPLIT, true);
                    me->DespawnOrUnsummon(4000);
                }
            }

            private:
                uint32 _timerExplotion;
                bool _isCasted;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_arc_chrono_shard_AI(creature);
        }
};

class spell_vandros_force_bomb : public SpellScriptLoader
{
    public:
        spell_vandros_force_bomb() : SpellScriptLoader("spell_vandros_force_bomb")
        {}

        class spell_force_bomb_SpellScript : public SpellScript
        {
            public:
                PrepareSpellScript(spell_force_bomb_SpellScript);

                void HandleDummy(SpellEffIndex /**/)
                {
                    if (!GetHitUnit())
                        return;
                    
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_FORCE_BOMB_AREA, true);
                }

                void Register() override
                {
                    OnEffectHitTarget += SpellEffectFn(spell_force_bomb_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_force_bomb_SpellScript();
        }
};

class spell_vandros_unstable_mana : public SpellScriptLoader
{
    public:
        spell_vandros_unstable_mana() : SpellScriptLoader("spell_vandros_unstable_mana")
        {}

        class spell_unstable_mana_AuraScript : public AuraScript
        {
            public:
                PrepareAuraScript(spell_unstable_mana_AuraScript);
        
                void HandlePeriodic(AuraEffect const* /**/)
                {
                    if (!GetUnitOwner())
                        return;
                    
                    Unit*&& owner = GetUnitOwner();

                    owner->CastSpell(owner, SPELL_UNSTABLE_MANA_DMG, true);
                }

                void Register() override
                {
                    OnEffectPeriodic += AuraEffectPeriodicFn(spell_unstable_mana_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_unstable_mana_AuraScript();
        }
};

class spell_vandros_banish_in_time_tele : public SpellScriptLoader
{
    public:
        spell_vandros_banish_in_time_tele() : SpellScriptLoader("spell_vandros_banish_in_time_tele")
        {
        }
        
        class spell_banish_in_time_tele_SpellScript : public SpellScript
        {
            public:
                PrepareSpellScript(spell_banish_in_time_tele_SpellScript);

                void HandleTeleport()
                {
                    WorldLocation pos;

                    pos.Relocate(VandrosTeleportLocations[ROOM_POSITION]);
                    
                    SetExplTargetDest(pos);
                }

                void Register() override
                {
                    BeforeCast += SpellCastFn(spell_banish_in_time_tele_SpellScript::HandleTeleport);
                }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_banish_in_time_tele_SpellScript();
        }
};

class spell_arc_breach : public SpellScriptLoader
{
    public:
        spell_arc_breach() : SpellScriptLoader("spell_arc_breach")
        {}

        class spell_arc_breach_AuraScript :  public AuraScript
        {
            public:
                PrepareAuraScript(spell_arc_breach_AuraScript);

                void HandlePeriodic(AuraEffect const* /**/)
                {
                    if (!GetUnitOwner())
                        return;
                    
                    GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_BREACH_DMG, true);
                }

                void Register() override
                {
                    OnEffectPeriodic += AuraEffectPeriodicFn(spell_arc_breach_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_arc_breach_AuraScript();
        }
};

void AddSC_boss_advisor_vandros()
{
    new boss_advisor_vandros();
    new npc_arc_chrono_shard();
    new spell_vandros_force_bomb();
    new spell_vandros_banish_in_time_tele();
    new spell_vandros_unstable_mana();
    new spell_arc_breach();
}
