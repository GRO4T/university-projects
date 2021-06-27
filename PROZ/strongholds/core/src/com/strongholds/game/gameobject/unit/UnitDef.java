package com.strongholds.game.gameobject.unit;

public class UnitDef {
    public float acceleration;
    public float maxVelocity;

    public int attackSpeed;
    public int timeBetweenAttacks;

    public int damage;
    public float range;

    public int maxHealth;

    public UnitDef(float acceleration, float maxVelocity, int attackSpeed, int timeBetweenAttacks, int damage, float range, int maxHealth) {
        this.acceleration = acceleration;
        this.maxVelocity = maxVelocity;
        this.attackSpeed = attackSpeed;
        this.timeBetweenAttacks = timeBetweenAttacks;
        this.damage = damage;
        this.range = range;
        this.maxHealth = maxHealth;
    }

    public UnitDef(UnitDef unitDef) {
        this.acceleration = unitDef.acceleration;
        this.maxVelocity = unitDef.maxVelocity;
        this.attackSpeed = unitDef.attackSpeed;
        this.timeBetweenAttacks = unitDef.timeBetweenAttacks;
        this.damage = unitDef.damage;
        this.range = unitDef.range;
        this.maxHealth = unitDef.maxHealth;
    }
}
