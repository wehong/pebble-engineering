module.exports = [
   {
    "type": "heading",
    "defaultValue": "App Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Engineering-DTA Configuration"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Colors"
      },
      {
        "type": "color",
        "messageKey": "COLOR_BACKGROUND",
        "defaultValue": "0x000000",
        "label": "Background Color"
      },
      {
        "type": "color",
        "messageKey": "COLOR_LABEL",
        "defaultValue": "0xAAAAAA",
        "label": "Label Color"
      },
      {
        "type": "color",
        "messageKey": "COLOR_HOUR_MARKS",
        "defaultValue": "0xAAAAAA",
        "label": "Hour Mark Color"
      },
      {
        "type": "color",
        "messageKey": "COLOR_MINUTE_MARKS",
        "defaultValue": "0x555555",
        "label": "Minute Mark Color"
      },
      {
        "type": "color",
        "messageKey": "COLOR_HOUR_HAND",
        "defaultValue": "0xFFFFFF",
        "label": "Hour Hand Color"
      },
      {
        "type": "color",
        "messageKey": "COLOR_MINUTE_HAND",
        "defaultValue": "0xFF0000",
        "label": "Minute Hand Color"
      },
      {
        "type": "color",
        "messageKey": "COLOR_SECOND_HAND",
        "defaultValue": "0xFF0000",
        "label": "Second Hand Color"
      }
      /*{
        "type": "color",
        "messageKey": "COLOR_DIGITTIME",
        "defaultValue": "0xFFFFFF",
        "label": "Digit Time Color"
      },
      {
        "type": "color",
        "messageKey": "COLOR_DATE",
        "defaultValue": "0xFFFFFF",
        "label": "Date Color"
      },
      {
        "type": "color",
        "messageKey": "COLOR_TEMPERATURE",
        "defaultValue": "0xFFFFFF",
        "label": "Temperature Color"
      },
      {
        "type": "color",
        "messageKey": "COLOR_BATTERY",
        "defaultValue": "0xFFFFFF",
        "label": "Battery Color"
      }*/
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Features"
      },
      {
        "type": "toggle",
        "messageKey": "SHOW_NUMBERS",
        "label": "Numbers",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "SHOW_SECOND_HAND",
        "label": "Second Hand",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "SHOW_DATE",
        "label": "Date",
        "defaultValue": true
      },
      /*{
        "type": "toggle",
        "messageKey": "SHOW_TEMPERATURE",
        "label": "Temperature",
        "defaultValue": false
      },*/
      {
        "type": "toggle",
        "messageKey": "SHOW_DIGITTIME",
        "label": "Digit Time",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "SHOW_BATTERY",
        "label": "Battery",
        "defaultValue": true
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];