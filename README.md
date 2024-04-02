### Makefile Documentation

This Makefile compiles and runs one of two C++ programs, based on an argument provided by the user:

1. **`birthday_presents_party.cc`** (Argument: `1`)
2. **`atmospheric_temperature_reading_module.cc`** (Argument: `2`)

#### Prerequisites

- GNU Make
- g++ Compiler

#### How to Use

Execute one of the following commands in the directory containing your source code and Makefile:

- For `birthday_presents_party.cc`:
  ```sh
  make PROBLEM=1
  ```
- For `atmospheric_temperature_reading_module.cc`:
  ```sh
  make PROBLEM=2
  ```

#### Targets

- `all`: Compiles and runs the selected program.
- `build`: Compiles the selected source file.
- `clean`: Removes executables and object files.
- `run`: Executes the compiled program.

## Problem 1: minotaur_birthday

### Output Formatting:

#### Problem 1: minotaur_birthday

```
Enter the number of guests: N
[ guest logic explained here ]
Guest one replaced the cupcake N times.
We can guarantee all guests have visited the labyrinth! It took 32 turns to make this guarantee.
```

#### Problem 2: minotaur_vase - strategy 3:

```
Enter the number of guests: N
Guest 1 is waiting in line.
Guest 1 is viewing the vase.
Guest 1 has finished viewing the vase.
Guest 2 is waiting in line.
Guest 2 is viewing the vase.
Guest 2 has finished viewing the vase.
Guest . is waiting in line.
Guest . is viewing the vase.
Guest . has finished viewing the vase.
Guest N is waiting in line.
Guest N is viewing the vase.
Guest N has finished viewing the vase.
```

### Statement:

#### Problem 1: minotaur_birthday

To address the problem, I initially brainstormed solutions on a whiteboard and recognized that guests could encode their experiences through their decisions to eat, ignore, or replace the cake. This led me to the idea of appointing a counter to track the number of eaten cakes, allowing me to determine the number of unique guests who navigated the maze. With this strategy in place, the multithreaded implementation became straightforward. Each guest is represented by a thread that sequentially enters the maze, makes decisions, and exits. After exiting, it signals the next thread based on the Minotaur's selection. This process repeats until the designated counter thread confirms all have eaten the cake.

##### Guest Logic:

A guest consumes the cupcake on their first visit unless they're the counter, who eats it only once but replaces it thereafter. If the counter finds the cupcake missing (a sign another guest has visited), they replenish it, thereby tracking visits. This method counts visits through the counter's replacements, enabling them to confirm when all guests have visited the labyrinth. The counter is then able to answer the question "can you guarantee every guest has visited?".

#### Problem 2: minotaur_vase - strategy 3

##### Strategy Selection Advantages/Disadvantages:

###### Strategy 1: Open Door Check

- Advantages: Allows guests the freedom to explore and attempt entry at any time, enhancing the party's spontaneity.
- Disadvantages: May cause overcrowding and lacks guarantees for viewing

 the vase, potentially leading to frustration.

###### Strategy 2: Availability Sign

- Advantages: Clearly shows if the showroom is open, reducing wait times at the door and promoting efficient use.
- Disadvantages: Depends on guests to update the sign correctly, which might not always occur, and doesn't solve queuing issues.

###### Strategy 3: Queue System

- Advantages: Provides organized access and guarantees turns, minimizing door crowding.
- Disadvantages: Could result in long waits and reduce guests' time to enjoy the party.

##### Conclusion:

- The queue system (Strategy 3) is the most structured, ensuring orderly access but may increase wait times. The optimal strategy depends on the event's size and guests' preference for freedom or structure.
- The implementation was straightforward.

### Proof of Correctness:

#### Problem 1: minotaur_birthday

The program outputs which guests are visiting the maze and their corresponding decisions regarding the cupcake in that visit. Using these outputs, I was able to manually verify the logic through the outputs for N selections of 1 through 5. Twice.

#### Problem 2: minotaur_vase - strategy 3

The program outputs which guests are viewing the vase and which are waiting in line. Using these outputs, I was able to manually verify the logic through the outputs for N selections of 1 through 5. Twice. Because it was a simple queue, it was not difficult to verify.

### Experimental Evaluations:

#### Problem 1: minotaur_birthday

I quickly implemented a solution that logically worked, but it was initially suboptimal. My approach had threads continuously checking their turn using atomic variables, leading to 100% CPU usage due to constant looping. To mitigate this, I introduced a delay between checks, but soon recognized that a `std::condition_variable` was the ideal solution. This C++ feature enabled me to establish an event-based waiting system for threads, significantly reducing condition checks and allowing threads to wait efficiently for their turn.

#### Problem 2: minotaur_vase - strategy 3

This solution was able to implement a subset of all the same features as the previous program in an optimal fashion, utilizing the correct C++ features for each logical requirement.

---

The Minotaur’s birthday party was a success. The Minotaur received a lot of presents from his guests. The next day he decided to sort all of his presents and start writing “Thank you” cards. Every present had a tag with a unique number that was associated with the guest who gave it. Initially all of the presents were thrown into a large bag with no particular order. The Minotaur wanted to take the presents from this unordered bag and create a chain of presents hooked to each other with special links (similar to storing elements in a linked-list). In this chain (linked-list) all of the presents had to be ordered according to their tag numbers in increasing order. 

The Minotaur asked 4 of his servants to help him with creating the chain of presents and writing the cards to his guests. Each servant would do one of three actions in no particular order: 

1. Take a present from the unordered bag and add it to the chain in the correct location by hooking it to the predecessor’s link. The servant also had to make sure that the newly added present is also linked with the next present in the chain. 

2. Write a “Thank you” card to a guest and remove the present from the chain. To do so, a servant had to unlink the gift from its predecessor and make sure to connect the predecessor’s link with the next gift in the chain. 

3. Per the Minotaur’s request, check whether a gift with a particular tag was present in the chain or not; without adding or removing a new gift, a servant would scan through the chain and check whether a gift with a particular tag is already added to the ordered chain of gifts or not. As the Minotaur was impatient to get this task done quickly,

 he instructed his servants not to wait until all of the presents from the unordered bag are placed in the chain of linked and ordered presents. Instead, every servant was asked to alternate adding gifts to the ordered chain and writing “Thank you” cards. The servants were asked not to stop or even take a break until the task of writing cards to all of the Minotaur’s guests was complete. 
 
 After spending an entire day on this task the bag of unordered presents and the chain of ordered presents were both finally empty! Unfortunately, the servants realized at the end of the day that they had more presents than “Thank you” notes. What could have gone wrong? Can we help the Minotaur and his servants improve their strategy for writing “Thank you” notes? Design and implement a concurrent linked-list that can help the Minotaur’s 4 servants with this task. In your test, simulate this concurrent “Thank you” card writing scenario by dedicating 1 thread per servant and assuming that the Minotaur received 500,000 presents from his guests.

Each present was tagged with unique guest number who gave it to minotaur
Minotaur randomized order "placed in bag then pulled out"
He wants ascending order linked list "chain of presents"
Four threads "servants" that alternate:
    * Add present from bag and add it to predancessor's link
    * Write "thank you" and remove present from chain
They will randomly (idk timer?)
    * Check if gift exists is chain
Every servant alternates adding and writing thank you until all thank you's are done.
n = 500,000

Each