class Event {
  constructor(time_gmt, time_local, title, text) {
    this.time_gmt = time_gmt;
    this.time_local = time_local;
    this.title = title;
    this.text = text;
  }
}

var events = [];

export async function getLiveEvents() {
  events = [];

  const res = await fetch(
    "https://very-teddi-iad-9839f521.koyeb.app/getRecentEvents"
  );

  if (!res.ok) {
    console.error(`❌ HTTP ${res.status}: ${res.statusText}`);
    return;
  }

  const data = await res.json();

  let event = new Event(
    data[0].time_gmt,
    data[0].time_local,
    data[0].title,
    data[0].text
  );

  events.push(event);

  return events;
}
