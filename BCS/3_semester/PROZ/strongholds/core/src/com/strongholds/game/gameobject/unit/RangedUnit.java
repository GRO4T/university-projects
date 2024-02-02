package com.strongholds.game.gameobject.unit;

import com.strongholds.game.GameSingleton;
import com.strongholds.game.gameobject.GameObject;
import com.strongholds.game.gameobject.GameObjectDef;

import java.util.Iterator;
import java.util.TimerTask;

public class RangedUnit extends Unit {
    public RangedUnit(GameObjectDef objectDef, UnitDef unitDef) {
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
    private class AttackTask extends TimerTask {
        @Override
        public void run() {
            int targetSize = getTargets().size();
            // 'targets' contains units so we won't damage enemy base
            if (targetSize > 1) {
                Iterator it = getTargets().iterator();

                GameObject closestObject = (GameObject) it.next();
                float myPositionX = getPosition().x;
                float closestDistance = Math.abs(myPositionX - closestObject.getPosition().x);
                GameObject object;
                // find closest object
                while (it.hasNext()) {
                    object = (GameObject) it.next();
                    float newDistance = Math.abs(myPositionX - object.getPosition().x);

                    if (newDistance < closestDistance || closestObject == null
                            || closestObject.getType() == GameSingleton.ObjectType.BASE) {
                        closestObject = object;
                        closestDistance = newDistance;
                    }
                }

                if (closestObject != null && closestObject.isEnemy() && !closestObject.getId().equals("enemyBase")) {
                    int sign = randomGenerator.nextBoolean() == true ? 1 : -1;
                    int finalDamage = getDamage() + sign * randomGenerator.nextInt(4);
                    model.enemyUnitHit(closestObject.getId(), finalDamage);
                    closestObject.gotHit(finalDamage);
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
