package com.strongholds.game.gameobject.unit;

import com.strongholds.game.GameSingleton;
import com.strongholds.game.gameobject.GameObject;
import com.strongholds.game.gameobject.GameObjectDef;

import java.util.Iterator;
import java.util.TimerTask;

/**
 * Represents a melee unit
 */
public class MeleeUnit extends Unit {
    public MeleeUnit(GameObjectDef objectDef, UnitDef unitDef) {
        super(objectDef, unitDef);
    }

    public void update(){
        super.update();
    }

    public void attack(){
        setState(GameSingleton.ObjectState.ATTACKING);
        canAttack = false;
        AttackTask attackTask = new AttackTask();
        SuspendAttackTask suspendAttackTask = new SuspendAttackTask();
        attackTimer.schedule(attackTask, getAttackSpeed());
        attackTimer.schedule(suspendAttackTask, getTimeBetweenAttacks());
    }

    /**
     * Task called when unit is about to attack.
     * After attackSpeed milliseconds it checks target list
     * and if there are any enemies or enemy bases in it,
     * deals damage to them.
     */
    private class AttackTask extends TimerTask{
        @Override
        public void run() {
            int targetSize = getTargets().size();
            // targets contain units so we won't damage enemy base
            if (targetSize > 1){
                Iterator it = getTargets().iterator();
                while (it.hasNext()){
                    GameObject object = (GameObject) it.next();
                    //attack only if it is an enemy unit or if it is an enemy base but there was no units before
                    if (object.isEnemy() && !object.getId().equals("enemyBase")){
                        int sign = randomGenerator.nextBoolean() == true ? 1 : -1;
                        int finalDamage = getDamage() + sign * randomGenerator.nextInt(4);
                        model.enemyUnitHit(object.getId(), finalDamage);
                        object.gotHit(finalDamage);
                    }
                }
            }
            else if (targetSize == 1){
                GameObject object = getTargets().get(0);
                if (object.isEnemy()){
                    int sign = randomGenerator.nextBoolean() == true ? 1 : -1;
                    int finalDamage = getDamage() + sign * randomGenerator.nextInt(4);
                    model.enemyUnitHit(object.getId(), finalDamage);
                    object.gotHit(finalDamage);
                }
            }

            if (Math.abs(getVelocity().x) < 0.5f){
                setState(GameSingleton.ObjectState.IDLING);
            }
            else{
                setState(GameSingleton.ObjectState.MOVING);
            }
        }
    }

}
