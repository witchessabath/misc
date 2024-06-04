var border_l = 720 - (current_width / 2);
var border_r = 720 + (current_width / 2);
var border_u = 484 - current_height;
var border_d = 484;

obj_soul.x = clamp(obj_soul.x, border_l, border_r);
obj_soul.y = clamp(obj_soul.y, border_u, border_d);
draw_set_color(c_white);
draw_rectangle(border_l - 4, border_u - 4, border_r + 4, border_d + 4, false);
draw_set_color(c_black);
draw_rectangle(border_l, border_u, border_r, border_d, false);

draw_set_font(font_battleui_hp);
draw_set_color(c_white);
draw_text(430, 500, string(global.Name));
draw_text(650, 500, string("LVL ") + string(global.PlayerLV));
draw_text(880, 500, string("HP ") + string(global.PlayerHP) + " / " + string(global.PlayerMaxHP));