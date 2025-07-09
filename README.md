# StateMachineSystem
A flexible and extensible State Machine system built for Unreal Engine, designed to support multiple concurrent state machines, custom transitions, input-driven logic, and Blueprint/C++ integration. Includes a sample project with a playable character demonstrating movement, jumping, shooting, and state communication.

**Plugin full documentation:** [link]



# 🔧 How to Install

To integrate the **State Machine System Plugin** into your Unreal Engine project:

1. **Download the Plugin**
    - Go to the Releases section of this repository.
    - Download the latest release `.zip` file.
2. **Add to Your Project**
    - Unzip the contents into your project’s `Plugins/` folder.
        
        If the folder doesn’t exist, create it.
        
    
    ```
    css
    CopiaModifica
    YourProject/
    └── Plugins/
        └── StateMachineSystem/
            └── [Plugin Files]
    
    ```
    
3. **Regenerate Project Files (if using C++)**
    - Right-click on your `.uproject` file and select **Generate Visual Studio project files**.
4. **Enable the Plugin**
    - Open your project in Unreal Engine.
    - Go to **Edit → Plugins**, search for `"Modular State Machine"` and ensure it is **enabled**.
    - Restart the editor if prompted.
5. **(Optional) Example Map**
    - The plugin includes a fully functional example map (`StateMachine_Example`) showing a character with movement, jump, and shooting states.
    - You can find it in the `Content/ModularStateMachine/Example/` folder.

> 💡 Note: The example scene also uses an optional camera plugin. It is not required for the plugin to work and can be safely disabled, but the example scene will no longer function correctly without it.
>



# Initial Setup

To get started with the State Machine System, follow these steps to set up your components and define your first states.

### **1 Add Components to Your Actor**

1. **Add a `UMultiStateMachineComponent`** to your Actor or Character.
    
    This component manages multiple independent state machines.
    
    ![AddComponent](https://github.com/user-attachments/assets/b6f61863-d8e6-4b01-ac84-afe439338e18)


    
2. **Within the Multi-State Machine**, add one or more `UStateMachineComponent` instances.
    
    Each `UStateMachineComponent` is responsible for running a single state machine with its own logic and states.
    
    ![MultiStateMahcineSetUp](https://github.com/user-attachments/assets/870e3902-de7b-4bd9-88b9-599a187065e6)
    

> 💡 Both components are ActorComponent-based and can be added in Blueprints or C++.
> 

---

### **2 Define Your States**

States are subclasses of `UStateBase`. Each one defines its own behavior and possible transitions.

- Create new Blueprint or C++ classes that inherit from `UStateBase`.
- **Set the `StateTag`** to uniquely identify this state (e.g., `State.Movement.Jump`).

  ![StateTag](https://github.com/user-attachments/assets/835c453f-e301-42dd-80e9-c29a45dd3af0)

- Implement the following core events to define state logic:
    - `OnInitialize(Context)` – Called when the state is initialized.
    - `OnEnter()` – Called when the state is activated.
    - `OnUpdate(DeltaTime)` – Called every tick while active.
    - `OnExit()` – Called when transitioning away from the state.
    - `OnHandleInput(InputActionTag, Value)` – Used to respond to input events.
    - `OnInterrupt(InterrupterTag)` – Handles interruptions from other states.
    
    ![StateEventOverride](https://github.com/user-attachments/assets/96d65930-4e24-493d-bf19-0d1cd04c5fb8)

    

---

### **3 Configure the State Machine**

In your `UStateMachineComponent`:

- **Set the `StateMachineTag`** to uniquely identify this state machine (e.g., `State.Movement`).
- **Define the `EntryStateClass`**, the state that will be active when the game begins or the machine is reset.
- **Add your custom `StateClasses`**, which will be registered and instantiated at runtime.

  ![StateMachineSetUp](https://github.com/user-attachments/assets/a901ae39-a8ec-4b9d-a5b6-ae99b45a7bc8)


> Each state will be automatically initialized and stored in an internal map using the StateTag you define in each UStateBase.
> 

---

### **4 Setup Transitions Between States**

Each `UStateBase` can define transitions using a map of `FGameplayTag -> FTransitionData`.

- **Key**: An `InputActionTag` (e.g., `Input.Jump`)
- **Value**: A `FTransitionData` struct containing:
    - `ToState` – The target state tag.
    - `TransitionEvents` – Optional logic to run during the transition.

  ![Transitions](https://github.com/user-attachments/assets/4ec4ea4f-dcb9-4ee8-9da9-6409a2e89fe3)


---

Once these steps are completed, your system is ready to be driven either by input or logic (e.g., AI decision-making).
