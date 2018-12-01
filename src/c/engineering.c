#include <pebble.h>
#include <inttypes.h>
#include "engineering.h"

static Window *window;
static Layer *s_simple_bg_layer, *s_hands_layer, *s_date_layer, /**s_temp_layer,*/ *s_connect_layer, *s_digittime_layer, *s_battery_layer, *s_bot_bg_layer;
//static TextLayer *s_day_label, *s_num_label;

static GPath *s_minute_arrow, *s_hour_arrow;
static char s_date_buffer[7], s_date_buffer_2[4], /*s_temp_buffer[5],*/ s_digittime_buffer[8], s_battery_buffer[4];
static int s_battery_level;

struct tm *s_tick_time;

//static AppSync s_sync;
//static uint8_t s_sync_buffer[64];

static GColor gcolor_background, gcolor_hour_marks, gcolor_minute_marks, gcolor_numbers, gcolor_hour_hand, gcolor_minute_hand, gcolor_second_hand;
static GColor gcolor_digittime, gcolor_date, /*gcolor_temperature,*/ gcolor_battery;
static bool b_show_numbers, /*b_show_temperature,*/ b_show_date, b_show_second_hand, b_show_digittime, b_show_battery;

static void load_persisted_values() {
	// SHOW_NUMBERS
	if (persist_exists(MESSAGE_KEY_SHOW_NUMBERS)) {
	  b_show_numbers = persist_read_int(MESSAGE_KEY_SHOW_NUMBERS);
	}

	// SHOW_SECOND_HAND
	if (persist_exists(MESSAGE_KEY_SHOW_SECOND_HAND)) {
	  b_show_second_hand = persist_read_int(MESSAGE_KEY_SHOW_SECOND_HAND);
	}

	// SHOW_TEMPERATURE
	/*if (persist_exists(MESSAGE_KEY_SHOW_TEMPERATURE)) {
	  b_show_temperature = persist_read_int(MESSAGE_KEY_SHOW_TEMPERATURE);
	}*/

	// SHOW_DATE
	if (persist_exists(MESSAGE_KEY_SHOW_DATE)) {
	  b_show_date = persist_read_int(MESSAGE_KEY_SHOW_DATE);
    layer_set_hidden(s_date_layer, !b_show_date);
	}
  
  // SHOW_DIGITTIME
	if (persist_exists(MESSAGE_KEY_SHOW_DIGITTIME)) {
	  b_show_digittime = persist_read_int(MESSAGE_KEY_SHOW_DIGITTIME);
    layer_set_hidden(s_digittime_layer, !b_show_digittime);
	}
  
  // SHOW_BATTERY
	if (persist_exists(MESSAGE_KEY_SHOW_BATTERY)) {
	  b_show_digittime = persist_read_int(MESSAGE_KEY_SHOW_BATTERY);
    layer_set_hidden(s_battery_layer, !b_show_battery);
	}

	// COLOR_BACKGROUND
	if (persist_exists(MESSAGE_KEY_COLOR_BACKGROUND)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_BACKGROUND);
		gcolor_background = GColorFromHEX(color_hex);
		window_set_background_color(window, gcolor_background);
	}

	// COLOR_HOUR_MARKS
	if (persist_exists(MESSAGE_KEY_COLOR_HOUR_MARKS)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_HOUR_MARKS);
		gcolor_hour_marks = GColorFromHEX(color_hex);
	}

	// COLOR_MINUTE_MARKS
	if (persist_exists(MESSAGE_KEY_COLOR_MINUTE_MARKS)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_MINUTE_MARKS);
		gcolor_minute_marks = GColorFromHEX(color_hex);
	}

	// COLOR_LABEL
	if (persist_exists(MESSAGE_KEY_COLOR_LABEL)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_LABEL);
		gcolor_numbers = GColorFromHEX(color_hex);
	}

	// COLOR_HOUR_HAND
	if (persist_exists(MESSAGE_KEY_COLOR_HOUR_HAND)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_HOUR_HAND);
		gcolor_hour_hand = GColorFromHEX(color_hex);
	}

	// COLOR_MINUTE_HAND
	if (persist_exists(MESSAGE_KEY_COLOR_MINUTE_HAND)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_MINUTE_HAND);
		gcolor_minute_hand = GColorFromHEX(color_hex);
	}

	// COLOR_SECOND_HAND
	if (persist_exists(MESSAGE_KEY_COLOR_SECOND_HAND)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_SECOND_HAND);
		gcolor_second_hand = GColorFromHEX(color_hex);
	}

	// COLOR_DIGITTIME
	if (persist_exists(MESSAGE_KEY_COLOR_DIGITTIME)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_DIGITTIME);
		gcolor_digittime = GColorFromHEX(color_hex);
    //APP_LOG(APP_LOG_LEVEL_INFO, "digittime color: %d", color_hex);
	}
  
  // COLOR_DATE
	if (persist_exists(MESSAGE_KEY_COLOR_DATE)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_DATE);
		gcolor_date = GColorFromHEX(color_hex);
	}
  
  // COLOR_TEMPERATURE
	/*if (persist_exists(MESSAGE_KEY_COLOR_TEMPERATURE)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_TEMPERATURE);
		gcolor_temperature = GColorFromHEX(color_hex);
	}*/
  
  // COLOR_BATTERY
	if (persist_exists(MESSAGE_KEY_COLOR_BATTERY)) {
		int color_hex = persist_read_int(MESSAGE_KEY_COLOR_BATTERY);
		gcolor_battery = GColorFromHEX(color_hex);
	}
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
 	/*Tuple *temperature_t = dict_find(iter, MESSAGE_KEY_TEMPERATURE);
 	if(temperature_t) {
		snprintf(s_temp_buffer, 5, "%d°", temperature_t->value->int16);
		//APP_LOG(APP_LOG_LEVEL_INFO, s_temp_buffer);
 	}*/

	Tuple *show_numbers_t = dict_find(iter, MESSAGE_KEY_SHOW_NUMBERS);
	if(show_numbers_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Show numbers %d", show_numbers_t->value->uint8);
 		b_show_numbers = show_numbers_t->value->uint8;
		persist_write_int(MESSAGE_KEY_SHOW_NUMBERS, b_show_numbers);
 	}

	Tuple *show_date_t = dict_find(iter, MESSAGE_KEY_SHOW_DATE);
	if(show_date_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Show date %d", show_date_t->value->uint8);
 		b_show_date = show_date_t->value->uint8;
		persist_write_int(MESSAGE_KEY_SHOW_DATE, show_date_t->value->uint8);
    layer_set_hidden(s_date_layer, !b_show_date);
 	}

	/*Tuple *show_temperature_t = dict_find(iter, MESSAGE_KEY_SHOW_TEMPERATURE);
	if(show_temperature_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Show temperature %d", show_temperature_t->value->uint8);
 		b_show_temperature = show_temperature_t->value->uint8;
		persist_write_int(MESSAGE_KEY_SHOW_TEMPERATURE, b_show_temperature);
    layer_set_hidden(s_temp_layer, !b_show_temperature);
 	}*/
  
  Tuple *show_digittime_t = dict_find(iter, MESSAGE_KEY_SHOW_DIGITTIME);
	if(show_digittime_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Show digittime %d", show_digittime_t->value->uint8);
 		b_show_digittime = show_digittime_t->value->uint8;
		persist_write_int(MESSAGE_KEY_SHOW_DIGITTIME, show_digittime_t->value->uint8);
    layer_set_hidden(s_digittime_layer, !b_show_digittime);
 	}
  
  Tuple *show_battery_t = dict_find(iter, MESSAGE_KEY_SHOW_BATTERY);
	if(show_battery_t) {
 		b_show_battery = show_battery_t->value->uint8;
		persist_write_int(MESSAGE_KEY_SHOW_BATTERY, show_battery_t->value->uint8);
    layer_set_hidden(s_battery_layer, !b_show_battery);
 	}

	Tuple *show_second_hand_t = dict_find(iter, MESSAGE_KEY_SHOW_SECOND_HAND);
	if(show_second_hand_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Show second hand %d", show_second_hand_t->value->uint8);
 		b_show_second_hand = show_second_hand_t->value->uint8;
		persist_write_int(MESSAGE_KEY_SHOW_SECOND_HAND, show_second_hand_t->value->uint8);
 	}

	Tuple *color_background_t = dict_find(iter, MESSAGE_KEY_COLOR_BACKGROUND);
	if(color_background_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Background color %lu", color_background_t->value->int32);
 		gcolor_background = GColorFromHEX(color_background_t->value->int32);
		window_set_background_color(window, gcolor_background);
		persist_write_int(MESSAGE_KEY_COLOR_BACKGROUND, color_background_t->value->int32);
 	}

	Tuple *color_label_t = dict_find(iter, MESSAGE_KEY_COLOR_LABEL);
	if(color_label_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Label color %lu", color_label_t->value->int32);
 		gcolor_numbers = GColorFromHEX(color_label_t->value->int32);
		persist_write_int(MESSAGE_KEY_COLOR_LABEL, color_label_t->value->int32);
 	}

	Tuple *color_hour_marks_t = dict_find(iter, MESSAGE_KEY_COLOR_HOUR_MARKS);
	if(color_hour_marks_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Hour mark color %lu", color_hour_marks_t->value->int32);
 		gcolor_hour_marks = GColorFromHEX(color_hour_marks_t->value->int32);
		persist_write_int(MESSAGE_KEY_COLOR_HOUR_MARKS, color_hour_marks_t->value->int32);
 	}

	Tuple *color_minute_marks_t = dict_find(iter, MESSAGE_KEY_COLOR_MINUTE_MARKS);
	if(color_minute_marks_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Minute mark color %lu", color_minute_marks_t->value->int32);
 		gcolor_minute_marks = GColorFromHEX(color_minute_marks_t->value->int32);
		persist_write_int(MESSAGE_KEY_COLOR_MINUTE_MARKS, color_minute_marks_t->value->int32);
 	}

	Tuple *color_hour_hand_t = dict_find(iter, MESSAGE_KEY_COLOR_HOUR_HAND);
	if(color_hour_hand_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Hour hand color %lu", color_hour_hand_t->value->int32);
 		gcolor_hour_hand = GColorFromHEX(color_hour_hand_t->value->int32);
		persist_write_int(MESSAGE_KEY_COLOR_HOUR_HAND, color_hour_hand_t->value->int32);
 	}

	Tuple *color_minute_hand_t = dict_find(iter, MESSAGE_KEY_COLOR_MINUTE_HAND);
	if(color_minute_hand_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Minute hand color %lu", color_minute_hand_t->value->int32);
 		gcolor_minute_hand = GColorFromHEX(color_minute_hand_t->value->int32);
		persist_write_int(MESSAGE_KEY_COLOR_MINUTE_HAND, color_minute_hand_t->value->int32);
 	}

	Tuple *color_second_hand_t = dict_find(iter, MESSAGE_KEY_COLOR_SECOND_HAND);
	if(color_second_hand_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Second hand color %lu", color_second_hand_t->value->int32);
 		gcolor_second_hand = GColorFromHEX(color_second_hand_t->value->int32);
		persist_write_int(MESSAGE_KEY_COLOR_SECOND_HAND, color_second_hand_t->value->int32);
 	}
  
  Tuple *color_digittime_t = dict_find(iter, MESSAGE_KEY_COLOR_DIGITTIME);
	if(color_digittime_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "digittime color: %lu", color_second_hand_t->value->int32);
 		gcolor_digittime = GColorFromHEX(color_digittime_t->value->int32);
		persist_write_int(MESSAGE_KEY_COLOR_DIGITTIME, color_digittime_t->value->int32);
    layer_mark_dirty(s_digittime_layer);
 	}
  
  Tuple *color_date_t = dict_find(iter, MESSAGE_KEY_COLOR_DATE);
	if(color_date_t) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Second hand color %lu", color_second_hand_t->value->int32);
 		gcolor_date = GColorFromHEX(color_date_t->value->int32);
		persist_write_int(MESSAGE_KEY_COLOR_DATE, color_date_t->value->int32);
    layer_mark_dirty(s_date_layer);
 	}
  
  /*Tuple *color_temperature_t = dict_find(iter, MESSAGE_KEY_COLOR_TEMPERATURE);
	if(color_temperature_t) {
 		gcolor_temperature = GColorFromHEX(color_temperature_t->value->int32);
		persist_write_int(MESSAGE_KEY_COLOR_TEMPERATURE, color_temperature_t->value->int32);
    layer_mark_dirty(s_temp_layer);
 	}*/
  
  Tuple *color_battery_t = dict_find(iter, MESSAGE_KEY_COLOR_BATTERY);
	if(color_battery_t) {
 		gcolor_battery = GColorFromHEX(color_battery_t->value->int32);
		persist_write_int(MESSAGE_KEY_COLOR_BATTERY, color_battery_t->value->int32);
    layer_mark_dirty(s_battery_layer);
 	}
  
  //layer_mark_dirty(window_get_root_layer(window));
  //layer_mark_dirty(s_simple_bg_layer);
  //layer_mark_dirty(s_hands_layer);
}

/*static bool color_hour_marks(GDrawCommand *command, uint32_t index, void *context) {
  gdraw_command_set_stroke_color(command, gcolor_hour_marks);
  return true;
}*/

/*static bool color_minute_marks(GDrawCommand *command, uint32_t index, void *context) {
  gdraw_command_set_stroke_color(command, gcolor_minute_marks);
  return true;
}*/


static int32_t get_angle_for_hour(int hour) {
  // Progress through 12 hours, out of 360 degrees
  return (hour * 360) / 12;
}

static int32_t get_angle_for_minute(int hour) {
  // Progress through 60 miunutes, out of 360 degrees
  return (hour * 360) / 60;
}


static void bg_update_proc(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);
  //APP_LOG(APP_LOG_LEVEL_INFO, "rect: %d, %d, %d, %d", 0/*bounds.x*/, 0/*bounds.y*/, bounds.size.w, bounds.size.h);
  //bounds.size.h -= 24;
	GRect frame = grect_inset(bounds, GEdgeInsets(4 * INSET));
	GRect inner_hour_frame = grect_inset(bounds, GEdgeInsets((4 * INSET) + 5));
	GRect inner_minute_frame = grect_inset(bounds, GEdgeInsets((4 * INSET) + 4));
	
	graphics_context_set_stroke_color(ctx, gcolor_hour_marks);
	graphics_context_set_stroke_width(ctx, 3);

	// Hours marks
	for(int i = 0; i < 12; i++) {
		int hour_angle = get_angle_for_hour(i);
		GPoint p0 = gpoint_from_polar(frame, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(hour_angle));
		GPoint p1 = gpoint_from_polar(inner_hour_frame, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(hour_angle));
		graphics_draw_line(ctx, p0, p1);
	}
	
	// Minute Marks
	graphics_context_set_stroke_color(ctx, gcolor_minute_marks);
	graphics_context_set_stroke_width(ctx, 1);
	for(int i = 0; i < 60; i++) {
		if (i % 5) {
			int minute_angle = get_angle_for_minute(i);
			GPoint p0 = gpoint_from_polar(frame, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(minute_angle));
			GPoint p1 = gpoint_from_polar(inner_minute_frame, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(minute_angle));
			graphics_draw_line(ctx, p0, p1);
		}
	}
	  
	// numbers
	if (b_show_numbers) {
		graphics_context_set_text_color(ctx, gcolor_numbers);
		
#ifdef PBL_RECT
		/*graphics_draw_text(ctx, "12", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(62, 9, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "1", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(86, 19, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "2", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(108, 40, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "3", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(117, 66, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "4", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(107, 95, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "5", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(86, 116, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "6", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(62, 124, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "7", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(39, 117, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "8", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(16, 95, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "9", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(7, 66, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "10", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(14, 40, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "11", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(34, 19, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);*/
    
    graphics_draw_text(ctx, "12", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(62, -1, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "1", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(86, 9, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "2", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(108, 28, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "3", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(117, 56, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "4", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(107, 84, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "5", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(85, 105, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "6", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(62, 113, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "7", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(39, 105, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "8", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(16, 84, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "9", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(7, 56, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "10", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(14, 28, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "11", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(34, 9, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
#else
		graphics_draw_text(ctx, "12", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(80, 10, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "1", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(107, 20, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "2", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(130, 43, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "3", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(140, 74, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "4", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(130, 106, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "5", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(107, 126, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
		graphics_draw_text(ctx, "6", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(81, 136, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "7", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(53, 124, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "8", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(29, 106, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "9", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(20, 74, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "10", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(28, 42, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
		graphics_draw_text(ctx, "11", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(50, 22, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
#endif
	}
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);
	GPoint center = grect_center_point(&bounds);
	int16_t second_hand_length = bounds.size.w / 2 - 5;
	int16_t second_hand_tail_length = 15;

	//time_t now = time(NULL);
	//struct tm *t = localtime(&now);
	
  int32_t second_angle = TRIG_MAX_ANGLE * s_tick_time->tm_sec / 60;
	int32_t second_angle_tail = TRIG_MAX_ANGLE * (s_tick_time->tm_sec + 30) / 60;



	// date
	//if (b_show_date) {
		//graphics_context_set_text_color(ctx, gcolor_numbers);
		//int offset = !b_show_numbers * 10;
//#ifdef PBL_RECT
		//graphics_draw_text(ctx, s_date_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(80, 75, 40 + offset, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
//#else
		//graphics_draw_text(ctx, s_date_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(100, 78, 45 + offset, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
//#endif
	//}

  // digittime
  //if (b_show_digittime) {
    //graphics_context_set_text_color(ctx, gcolor_numbers);
    //strftime(s_digittime_buffer, sizeof(s_digittime_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", t);
//#ifdef PBL_RECT
    //graphics_draw_text(ctx, s_digittime_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(53, 93, 40, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
//#else
    //graphics_draw_text(ctx, s_digittime_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(66, 110, 50, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
//#endif
//  }
    
	// temperature
	//if (b_show_temperature) {
		//graphics_context_set_text_color(ctx, gcolor_numbers);
		//int offset = !b_show_numbers * 10;
//#ifdef PBL_RECT
		//graphics_draw_text(ctx, s_temp_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(27 - offset, 75, 40 + offset, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
//#else
		//graphics_draw_text(ctx, s_temp_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(40 - offset, 78, 40 + offset, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
//#endif
	//}

	// minute hand
	graphics_context_set_fill_color(ctx, gcolor_minute_hand);
	gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * s_tick_time->tm_min / 60);
	gpath_draw_filled(ctx, s_minute_arrow);

	// hour hand
	graphics_context_set_fill_color(ctx, gcolor_hour_hand);
	gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (((s_tick_time->tm_hour % 12) * 6) + (s_tick_time->tm_min / 10))) / (12 * 6));
	gpath_draw_filled(ctx, s_hour_arrow);

	// second hand
	if (b_show_second_hand) {
    
    GPoint second_hand = {
		  .x = (int16_t)(sin_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.x,
		  .y = (int16_t)(-cos_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.y,
	  };
	  GPoint second_hand_tail = {
		  .x = (int16_t)(sin_lookup(second_angle_tail) * (int32_t)second_hand_tail_length / TRIG_MAX_RATIO) + center.x,
		  .y = (int16_t)(-cos_lookup(second_angle_tail) * (int32_t)second_hand_tail_length / TRIG_MAX_RATIO) + center.y,
	  };
    
		graphics_context_set_stroke_color(ctx, gcolor_second_hand);
		graphics_draw_line(ctx, second_hand, center);
		graphics_draw_line(ctx, second_hand_tail, center);
	}

	// dot in the middle
	graphics_context_set_fill_color(ctx, gcolor_second_hand);
	graphics_fill_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), 4);
  
  //graphics_context_set_stroke_color(ctx, gcolor_hour_marks);
	//graphics_context_set_stroke_width(ctx, 5);
  //graphics_draw_line(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), GPoint(105, 20));
}

char *uppercase(char *str) {
    for (int i = 0; str[i] != 0; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 0x20;
        }
    }

    return str;
}

static void date_update_proc(Layer *layer, GContext *ctx) {
	//time_t now = time(NULL);
	//struct tm *t = localtime(&now);
  
	graphics_context_set_text_color(ctx, gcolor_date);

#ifdef PBL_RECT
  strftime(s_date_buffer, sizeof(s_date_buffer), "%m%d", s_tick_time);
  strftime(s_date_buffer_2, sizeof(s_date_buffer_2), "%a", s_tick_time);
	uppercase(s_date_buffer_2);
	//graphics_draw_text(ctx, s_date_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(80, 75, 40 + offset, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  //graphics_draw_text(ctx, s_date_buffer_1, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(90, 145, 50, 14), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
  graphics_draw_text(ctx, s_date_buffer_2, fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(5, 145, 50, 14), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  graphics_draw_text(ctx, s_date_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(99, 145, 40, 14), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
#else
 	int offset = !b_show_numbers * 10;
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %d", s_tick_time);
	uppercase(s_date_buffer);
	graphics_draw_text(ctx, s_date_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(100, 78, 45 + offset, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
#endif
}

static void dt_update_proc(Layer *layer, GContext *ctx) {
  //time_t now = time(NULL);
	//struct tm *t = localtime(&now);
 
  strftime(s_digittime_buffer, sizeof(s_digittime_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", s_tick_time);
  
  graphics_context_set_text_color(ctx, gcolor_digittime);
#ifdef PBL_RECT
  //graphics_draw_text(ctx, s_digittime_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(53, 93, 40, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  //graphics_draw_text(ctx, s_digittime_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(2, 0, 40, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx, s_digittime_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(52, 145, 40, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
#else
  graphics_draw_text(ctx, s_digittime_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(66, 100, 50, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
#endif
}

/*static void temp_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_text_color(ctx, gcolor_temperature);
	int offset = !b_show_numbers * 10;
#ifdef PBL_RECT
	graphics_draw_text(ctx, s_temp_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(27 - offset, 75, 40 + offset, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
#else
	graphics_draw_text(ctx, s_temp_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(40 - offset, 78, 40 + offset, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
#endif
}*/

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_text_color(ctx, gcolor_battery);
  //sprintf(s_battery_buffer, "%d", s_battery_level);
  itoa(s_battery_level, s_battery_buffer);
  
#ifdef PBL_RECT
  //graphics_draw_text(ctx, s_battery_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(110, 8, 30, 14), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
  graphics_draw_text(ctx, s_battery_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(110, 0, 30, 14), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
#else
  graphics_draw_text(ctx, s_battery_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(55, 78, 30, 14), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
#endif
}

static void battery_callback(BatteryChargeState state) {
  // Record the new battery level
  s_battery_level = state.charge_percent;
  if (b_show_battery) {
    layer_mark_dirty(s_battery_layer);
  }
}

static void connect_update_proc(Layer *layer, GContext *ctx) {
#ifdef PBL_COLOR
  graphics_context_set_text_color(ctx, GColorRed);
#else
  graphics_context_set_text_color(ctx, gcolor_battery);
#endif
  
#ifdef PBL_RECT
  graphics_draw_text(ctx, "X", fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(7, 0, 30, 14), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
#else
  graphics_draw_text(ctx, "X", fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(75, 45, 30, 14), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
#endif
}

static void bluetooth_callback(bool connected) {
  // Show icon if disconnected
  layer_set_hidden(s_connect_layer, connected);

  if(!connected) {
    // Issue a vibrating alert
    vibes_double_pulse();
  }
}

static void bot_bg_proc(Layer *layer, GContext *ctx) {
#ifdef PBL_COLOR
 	graphics_context_set_stroke_color(ctx, GColorDarkGray);
#else
  graphics_context_set_stroke_color(ctx, GColorWhite);
#endif
	graphics_context_set_stroke_width(ctx, 1);
  graphics_draw_round_rect(ctx, GRect(2, 149, 140, 17), 2);
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
	//layer_mark_dirty(window_get_root_layer(window));
  
  //time_t now = time(NULL);
	//s_tick_time = localtime(&now);
  *s_tick_time = *tick_time;
  
  layer_mark_dirty(s_hands_layer);
  if (b_show_date && s_tick_time->tm_sec == 00) {
    layer_mark_dirty(s_date_layer);
  }
  if (b_show_digittime && s_tick_time->tm_sec == 00) {
    layer_mark_dirty(s_digittime_layer);
  }
 	/*if (b_show_temperature) {
    layer_mark_dirty(s_temp_layer);
  }*/
}

/*static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	s_simple_bg_layer = layer_create(bounds);
	layer_set_update_proc(s_simple_bg_layer, bg_update_proc);
	layer_add_child(window_layer, s_simple_bg_layer);

	window_set_background_color(window, gcolor_background);

	s_date_layer = layer_create(bounds);
	layer_set_update_proc(s_date_layer, date_update_proc);
	layer_add_child(window_layer, s_date_layer);

	s_hands_layer = layer_create(bounds);
	layer_set_update_proc(s_hands_layer, hands_update_proc);
	layer_add_child(window_layer, s_hands_layer);

  s_digit_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  text_layer_set_text(s_digit_time_layer, "00:00");
  text_layer_set_font(s_digit_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  
	load_persisted_values();
}*/

static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
  GRect ubounds = bounds;
#ifdef PBL_RECT
  ubounds.origin.y += 1;
  ubounds.size.h -= 22;
#endif

	s_simple_bg_layer = layer_create(ubounds);
	layer_set_update_proc(s_simple_bg_layer, bg_update_proc);
	layer_add_child(window_layer, s_simple_bg_layer);

	window_set_background_color(window, gcolor_background);

 	s_date_layer = layer_create(bounds);
	layer_set_update_proc(s_date_layer, date_update_proc);
	layer_add_child(window_layer, s_date_layer);
  
  s_digittime_layer = layer_create(bounds);
  layer_set_update_proc(s_digittime_layer, dt_update_proc);
  layer_add_child(window_layer, s_digittime_layer);
  
  /*s_temp_layer = layer_create(bounds);
	layer_set_update_proc(s_temp_layer, temp_update_proc);
	layer_add_child(window_layer, s_temp_layer);*/
  
  s_battery_layer = layer_create(bounds);
	layer_set_update_proc(s_battery_layer, battery_update_proc);
	layer_add_child(window_layer, s_battery_layer);
  
  s_connect_layer = layer_create(bounds);
	layer_set_update_proc(s_connect_layer, connect_update_proc);
	layer_add_child(window_layer, s_connect_layer);
  
  s_hands_layer = layer_create(ubounds);
	layer_set_update_proc(s_hands_layer, hands_update_proc);
	layer_add_child(window_layer, s_hands_layer);

#ifdef PBL_RECT
  s_bot_bg_layer = layer_create(bounds);
  layer_set_update_proc(s_bot_bg_layer, bot_bg_proc);
  layer_add_child(window_layer, s_bot_bg_layer);
#endif
  
	load_persisted_values();
  
  layer_mark_dirty(window_get_root_layer(window));
}

static void window_unload(Window *window) {
	layer_destroy(s_simple_bg_layer);
	layer_destroy(s_date_layer);
  layer_destroy(s_digittime_layer);
  //layer_destroy(s_temp_layer);
  layer_destroy(s_battery_layer);
  layer_destroy(s_connect_layer);
	layer_destroy(s_hands_layer);
  layer_destroy(s_bot_bg_layer);
  
	//text_layer_destroy(s_day_label);
	//text_layer_destroy(s_num_label);
}

static void init() {	
	
	// Default colors
	gcolor_background = GColorBlack;
	gcolor_minute_hand = GColorWhite;
	
	#ifdef PBL_COLOR
		gcolor_hour_marks = GColorLightGray;
		gcolor_minute_marks = GColorDarkGray;
		gcolor_numbers = GColorLightGray;
		gcolor_hour_hand = GColorRed;
		gcolor_second_hand = GColorRed;
    gcolor_digittime = GColorLightGray;
    gcolor_date = GColorLightGray;
    //gcolor_temperature = GColorLightGray;
    gcolor_battery = GColorLightGray;
	#else
		gcolor_hour_marks = GColorWhite;
		gcolor_minute_marks = GColorWhite;
		gcolor_numbers = GColorWhite;
		gcolor_hour_hand = GColorWhite;
		gcolor_second_hand = GColorWhite;
    gcolor_digittime = GColorWhite;
    gcolor_date = GColorWhite;
    //gcolor_temperature = GColorWhite;
    gcolor_battery = GColorWhite;
	#endif
  
	b_show_numbers = true;
	b_show_second_hand = true;
	b_show_date = true;
	//b_show_temperature = true;
  b_show_digittime = true;
  b_show_battery = true;
  
  // Register for battery level updates
  battery_state_service_subscribe(battery_callback);
  
  // Register for Bluetooth connection updates
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
  
  // Register with TickTimerService
	tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
  
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});
	window_stack_push(window, true);

	//s_temp_buffer[0] = '\0';
	s_date_buffer[0] = '\0';
  s_date_buffer_2[0] = '\0';
  s_digittime_buffer[0] = '\0';
  s_battery_buffer[0] = '\0';

	// init hand paths
	s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
	s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);

	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
#ifdef PBL_RECT
  bounds.size.h -= 24;
#endif
	GPoint center = grect_center_point(&bounds);
	gpath_move_to(s_minute_arrow, center);
	gpath_move_to(s_hour_arrow, center);
  
  // init time
  time_t now = time(NULL);
	s_tick_time = localtime(&now);
  
  // Ensure battery level is displayed from the start
  battery_callback(battery_state_service_peek());
  
  // Show the correct state of the BT connection from the start
  bluetooth_callback(connection_service_peek_pebble_app_connection());

	app_message_register_inbox_received(inbox_received_handler);
	app_message_open(128, 128);
}

static void deinit() {
	gpath_destroy(s_minute_arrow);
	gpath_destroy(s_hour_arrow);

	tick_timer_service_unsubscribe();
	window_destroy(window);
}

int main() {
	init();
	app_event_loop();
	deinit();
}
