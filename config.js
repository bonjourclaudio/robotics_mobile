const config = {
  textToSpeechModel: -1, // -1: not tts, 0: en_GB-cori-high, 1:en_GB-alan-medium, 2:en_GB-alan-low, 3:en_US-lessac-medium, 4: de_DE-thorsten-medium. Changing this value may cause an initial delay as the model is downloaded.
  speechToTextModel: 0, // 0: small english, 1: medium english, 2: small german. 3:  giga english.  Changing this value may cause an initial delay as the model is downloaded.
  //------ alternatively, you can set speechToTextModel to a string with the model name
  // speechToTextModel: "vosk-model-small-de-0.15",
  //------ full list of TTS models can be found here:  https://alphacephei.com/vosk/models
  // notifyTTS: true, // if enabled, send a notification to the Arduino in the format
  volume: 0, // 0 to 100
  // OPENAI_API_KEY: 'your-api-key-here'

  // WiFi Configuration (optional)
  // The system will auto-detect the network type based on your credentials:

  // For regular WPA2/WPA3 networks (type auto-detected):
  // wifi: {
  //   ssid: "YourNetworkName",
  //   password: "YourNetworkPassword"
  // },

  // For WPA2 Enterprise networks (auto-detected when username provided):
  // wifi: {
  //   ssid: "YourEnterpriseNetwork",
  //   username: "your.username",
  //   password: "your.password"
  // },

  chatGPTSettings: {
    temperature: 0.2, //Number between -2.0 and 2.0 //Positive value decrease the model's likelihood to repeat the same line verbatim.
    frequency_penalty: 0.0, //Number between -2.0 and 2.0. //Positive values increase the model's likelihood to talk about new topics.
    presence_penalty: 0.0, //Number between -2.0 and 2.0. //Positive values increase the model's likelihood to generate words and phrases present in the input prompt
    model: "gpt-4.1", //gpt-4o-mini, gpt-4o, gpt-4, gpt-3.5-turbo, gpt-4.1-nano
    max_tokens: 8192, //Number between 1 and 8192. //The maximum number of tokens to generate in the completion. The token count of your prompt plus max_tokens cannot exceed the model's context length. Most models have a context length of 8192 tokens (except for the newest models, which can support more than 128k tokens).
    user_id: "1", //A unique identifier for the user. //This is used to track the usage of the API.
    url: "https://api.openai.com/v1/chat/completions",
  },
  communicationMethod: "Serial", //Serial or "BLE"

  // These are actions is things the LLM can do
  // The list of functions should match those set up on the arduino
  functions: {
    actions: {
      change_rotation_speed: {
        commType: "write",
        dataType: "number",
        description:
          "Changes the speed of the Rotation. Accepts value between 5 and 20 only.",
      },
      start_spin_1: {
        commType: "write",
        dataType: "number",
        description:
          "Initiate the movement of motor one. The speed is defined by a number between 30 and 200.",
      },
      start_spin_2: {
        commType: "write",
        dataType: "number",
        description:
          "Initiate the movement of motor two. The speed is defined by a number between 30 and 200.",
      },
      start_spin_3: {
        commType: "write",
        dataType: "number",
        description:
          "Initiate the movement of motor three. The speed is defined by a number between 30 and 200.",
      },
      start_spin_4: {
        commType: "write",
        dataType: "number",
        description:
          "Initiate the movement of motor four. The speed is defined by a number between 30 and 200.",
      },
      start_spin_5: {
        commType: "write",
        dataType: "number",
        description:
          "Initiate the movement of motor five. The speed is defined by a number between 30 and 200.",
      },
      stop_all_spins: {
        commType: "write",
        dataType: "string",
        description: "Stops the motor from spinning",
      },
    },
    notifications: {},

    frontEnd: {
      start_default_music: {
        dataType: "boolean",
        description:
          "0 is off, 1 is on. Start playing the default music tracks on the front end",
      },
      play_track: {
        dataType: "number",
        description:
          "Plays tracks from the lullaby. 1-8 are single tracks, 45 plays tracks 4 and 5 together, 12345678 plays all tracks together.",
      },
      stop_track: {
        dataType: "number",
        description: "stop a music track on the front end",
      },
    },

    scraper: {
      start_mobile: {
        dataType: "string",
        description: "Run XYZ",
      },
      checkFish: {
        dataType: "string",
        description:
          "Don't say anything. Just run the function play_track with the value 1.",
      },
    },
  },

  // assistant messages help store prior responses
  conversationProtocol: [
    {
      role: "system",
      content: `You are a deterministic installation controller.
You never chat or explain. You only call functions to control hardware and frontend.
Your job: read event headlines and decide which functions to call.

Rules:
- Never output text; only call functions.
- Use the following rubric:

  +3: attack, airstrike, explosion, rocket, shooting, massacre, people injured|killed|dead, casualties
  +2: storm, raid, clashes, violent confrontation (no casualties)
  +1: threats, escalation, tense standoff, verbal abuse, incendiary rhetoric
  0: analysis (“WATCH:”), diplomacy, praise, politics

- Deduplicate identical (title+timestamp) events.
- Total score → severity:
    ≥5→5, 3–4→4, 2→3, 1→2, 0→1

Mappings:
• Base rotation speed = 5 + (severity - 1) * (10 / 4); clamp 5–20
• If severity > 3:
    start_spin_1(200), start_spin_2(200), start_spin_3(200), start_spin_4(200), start_spin_5(-80)
  else:
    stop_all_spins
• Music (frontEnd.play_track):
    sev1→12345678, sev2→123456, sev3→1234, sev4→12, sev5→1
• Always call change_rotation_speed, then spin calls (or stop), then play_track — exactly once each turn.

If no violent terms appear, severity ≤2.  
No roleplay, no chatting, no waiting for confirmation. Act immediately.`,
    },
  ],
};
export { config };
