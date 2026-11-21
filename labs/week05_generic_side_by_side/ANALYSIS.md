# Concept Lab 5 — Generic and Non-Generic, Side by Side

**Name:**  Sylvia    **Date:** 11/21/2026

---

## Step 1 — Get both sides working.

```bash
cmake --preset course-debug
cmake --build --preset course-debug
./build/course-debug/generic_side_by_side
./build/course-debug/generic_side_by_side_tests
```

Every check in `generic_side_by_side_tests` must say `ok` before you fill in
the rest of this file. Record the line the test program prints last:

**Test result:** 0 check(s) failed.

**The two types I instantiated `Satchel<T>` for:**

| | Type | Where it is used |
|:--|:--|:--|
| 1 | | Stores loot item names and is used by LegacyLootBridge to load and store loot. |
| 2 | | Used in Satchel<int, 12> to demonstrate that the same template works with integers. |

A third instantiation earns nothing extra. Two is the criterion.

---

## Step 2 — What the template version made easier.

**This list may not be empty.** At least two entries, and every entry names
something concrete that you actually did or did not have to write.

Do not write "it's more reusable" or "it's type safe" and stop there. Those
are the headline, not the answer. The answer is the line of code you did not
have to write, or the mistake you could not have made.

Useful places to look:

- Compare the signature of `Satchel<T>::remove_at` with the signature of
  `raw_remove_at`. Count the arguments. Count the things that can be wrong.
- Count the validity checks at the top of each body.
- `Satchel<int, 12>` cost you how many additional lines of implementation?
- Which of the two could hold `Monster*` tomorrow without an edit?

| | What it made easier | The concrete evidence |
|:--|:--|:--|
| 1 | | Satchel::remove_at() only needs an index, while raw_remove_at() needs the array, count pointer, and index. |
| 2 | | The raw version checks for null pointers before removing an item. The template already manages its own storage and count, so I only had to check whether the index was valid. |
| 3 | | I used Satchel<std::string> and Satchel<int, 12> with the same implementation. I didn't need to create a separate class for integers. |

---

## Step 3 — What the raw version made easier.

**This list may not be empty either**, and this is the half that submissions
get wrong. A submission claiming the raw array was worse in every respect
loses the marks for this section. The raw table is still in the engine, it
is still there for reasons, and you are expected to be able to name them.

Look at:

- `print_loot_table()` indexes `LOOT_TABLE[i]` directly. What would the same
  line cost through a `Satchel`?
- The content team edits `LOOT_TABLE` in a text editor. What does the
  template version ask of them?
- How many bytes is `LOOT_TABLE`? How many is `Satchel<std::string>` holding
  the same twelve names? Count them — twelve `std::string` objects plus
  sixteen slots of capacity, against twelve pointers.
- Compile time, and what shows up in a debugger when you inspect each one.
- What happened to the error message the first time you got the template
  wrong? Paste the first line of it if you still have it.

| | What it made easier | The concrete evidence |
|:--|:--|:--|
| 1 | | With LOOT_TABLE[i], I can access an item directly. With Satchel, I have to use a function like at(i) instead. |
| 2 | | The loot names are already written in LOOT_TABLE, so I can change them directly without creating a Satchel or loading the items into one. |
| 3 | | LOOT_TABLE stores 12 pointers, which take 96 bytes on a typical 64-bit system. Satchel<std::string> has space for 16 string objects, so it uses more memory.|

---

## Step 4 — The bridge.

**Where in your `load()` is the text copied, and how many times?** Name the
line. If it is copied twice, say so — an honest "my version copies twice,
here, and here is what it would take to fix it" scores above a claim that it
does not.

*My answer:* In my load() function, the text is copied once on line 119 of legacy_loot.cpp, where I assign slots[i] to *slot using *slot = slots[i];. The add_slot() function gives me an empty slot in the satchel, so I don't need to create another string and copy it again. This avoids the extra copy that would happen if I used add() instead.

**`store()` hands out pointers into the satchel's own strings. Name one way
a caller could get a dangling pointer out of that, and the rule that
prevents it.**

*My answer:* A dangling pointer could happen if the satchel is destroyed while another part of the program is still using the pointers returned by store(). Those pointers would no longer point to valid string data. To prevent this, the satchel must stay alive, and its strings must not be modified or removed while those pointers are being used.

**The bridge is not allowed to touch `LOOT_TABLE`. If that constraint were
lifted — if you could rewrite the loot table into whatever shape you liked —
name one thing you would still not change, and why.**

*My answer:* Even if I could change LOOT_TABLE, I would still keep the original loot names and their order the same. Other parts of the game might depend on those values and their positions. Changing them could cause the game to display the wrong loot or behave differently than intended.

---

## Step 5 — The closing question.

**Build 5 adds `LegacyLootBridge` to the engine, against the same
unmodifiable loot table. Which of the two implementations in this lab is the
one you would actually put in Build 5, and what does the other one keep
doing after you ship it?**

The answer "the template one, it's better" earns nothing. Both are still in
the engine after Build 5; say which code calls which, and why that split is
the right one.

*My answer:* For Build 5, I would use the Satchel<std::string> template with LegacyLootBridge to manage loot items. The bridge would load the original loot names from LOOT_TABLE into the satchel, where items could be added or removed more easily. The raw LOOT_TABLE would still stay in the engine because existing code, such as print_loot_table(), uses it directly. This way, the new code can use the template without breaking the older parts of the game.

---

## What is being graded

| | |
|:--|:--|
| All checks in `generic_side_by_side_tests` pass | required |
| `Satchel<T>` instantiated for two or more types | required |
| Step 2 list non-empty, with concrete evidence | required |
| Step 3 list non-empty, with concrete evidence | required — this is the one submissions skip |
| `LOOT_TABLE` unmodified, and not deleted | required — deleting it fails the lab |
| Steps 4 and 5 | **the graded artifact** — passing tests alone is not |

Submit this file, completed, as directed on the assignment page.
