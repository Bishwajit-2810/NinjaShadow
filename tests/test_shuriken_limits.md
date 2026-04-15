# Shuriken System Tests

## Test Scenario 1: Shuriken Count Limit

**Expected Behavior:** Player can throw exactly 10 shurikens per level, no more

**Setup:**

1. Start Level 1
2. Verify HUD shows shuriken count = 10
3. Press X to throw shurikens
4. Count down to 0

**Pass Criteria:**

- ✅ HUD shows shuriken_count decreasing (10 → 9 → 8 ... → 1 → 0)
- ✅ After throwing 10 shurikens, no more shurikens appear on screen
- ✅ Count never goes below 0
- ✅ On new level, count resets to 10

---

## Test Scenario 2: Shuriken Range Limit (50 units)

**Expected Behavior:** Shurikens disappear after traveling 50 units from player

**Setup:**

1. Start Level 1
2. Throw a shuriken
3. Mark the shuriken's travel distance
4. Observe when it disappears

**Pass Criteria:**

- ✅ Shuriken travels ~50 units then disappears (range enforced)
- ✅ Shuriken velocity is constant (no slowdown before disappearing)
- ✅ Distance measured from player.x, not from throw point

---

## Test Scenario 3: Key Repeat Prevention

**Expected Behavior:** Holding X key throws only ONE shuriken, not multiple

**Setup:**

1. Start Level 1
2. Hold X key for 2 seconds
3. Count shurikens created
4. Release X, wait 1 sec, press X again

**Pass Criteria:**

- ✅ Holding X creates exactly 1 shuriken (not 10+ from key repeat)
- ✅ Shuriken count decreases by 1 only (not by 10)
- ✅ Pressing X again after release creates another single shuriken
- ✅ Rapid X presses (each followed by release) create 1 shuriken per press

---

## Test Scenario 4: Shuriken Behavior When Max Projectiles Reached

**Expected Behavior:** When level has 64 active projectiles, no new shurikens created

**Setup:**

1. Manually fill level.proj_count to MAX_PROJECTILES (64)
2. Press X to throw shuriken
3. Check if shuriken was created

**Pass Criteria:**

- ✅ Shuriken NOT created (blocked by MAX_PROJECTILES limit)
- ✅ Shuriken count NOT decremented (ammo preserved)
- ✅ After some projectiles disappear, can throw again

---

## Test Scenario 5: Shuriken Direction (Based on Player Facing)

**Expected Behavior:** Shuriken travels in direction player is facing

**Setup:**

1. Player faces right (vx positive)
2. Throw shuriken
3. Verify direction, then turn around
4. Repeat facing left

**Pass Criteria:**

- ✅ Shuriken.vx = player.facing \* 450 (positive right, negative left)
- ✅ Shuriken follows player facing direction correctly

---

## Bug Fixes Applied

1. **Key Repeat Prevention:**
   - Added `player.shuriken_pressed` tracking
   - Moved throw logic to `player_input()`
   - Set `shuriken_pressed = 0` after each throw
2. **Count Decrement Logic:**
   - Always decrements when throw succeeds
   - Check `level.proj_count < MAX_PROJECTILES` before creating
   - Skip throw if either condition fails (no ammo decrement)

3. **Range Enforcement:**
   - ✅ Already working: `range = 50.0f` set on creation
   - ✅ Already working: `if (fabsf(pr->x - player.x) > pr->range) pr->active = 0`

---

## Test Results

| Test                   | Status     | Notes                                     |
| ---------------------- | ---------- | ----------------------------------------- |
| Count Limit (10/level) | ⏳ PENDING | Compile succeeded, awaiting gameplay test |
| Range Limit (50 units) | ⏳ PENDING | Code verified, awaiting gameplay test     |
| Key Repeat Fix         | ⏳ PENDING | Fixed in code, needs verification         |
| Max Projectiles        | ⏳ PENDING | Should prevent throws when full           |
| Direction              | ⏳ PENDING | Player.facing \* 450 logic verified       |
