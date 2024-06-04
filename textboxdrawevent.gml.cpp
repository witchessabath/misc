accept_key = keyboard_check_pressed(vk_enter);

textbox_x = camera_get_view_x(view_camera[0] + 450);
textbox_y = camera_get_view_y(view_camera[0]) + 650;

if setup == false
	{
	setup = true;
	draw_set_font(font_textbox);
	draw_set_valign(fa_top);
	draw_set_halign(fa_left);
	
	page_number = array_length(text);
	for(var p = 0; p < page_number; p++)
		{
			
		text_length[p] = string_length(text[p]);
		
			text_x_offset[p] = 44;
		}
	}
	
if draw_char < text_length[page]
	{
	draw_char += text_spd;
	draw_char = clamp(draw_char, 0, text_length[page]);
	}


if accept_key
	{
	if draw_char == text_length[page]
		{
		if page < page_number-1
			{
			page++;
			draw_char = 0;
			}
		else
			{
			///link text for options
			if option_number > 0 {
				create_textbox(option_link_id[option_pos]);
			}
			instance_destroy();
			}
		}
	else
		{
		draw_char = text_length[page];
		}
	}

//draw textbox
var _txtb_x = textbox_x + text_x_offset[page];
var _txtb_y = textbox_y;
txtb_img += txtb_imgspd;
txtb_spr_w = sprite_get_width(txtb_spr);
txtb_spr_h = sprite_get_height(txtb_spr);
draw_sprite_ext(txtb_spr, txtb_img, _txtb_x, _txtb_y, textbox_width/txtb_spr_w, textbox_height/txtb_spr_h, 0, c_white, 1);

//options
if draw_char == text_length[page] && page == page_number - 1
	{
	option_pos += keyboard_check_pressed(vk_down) - keyboard_check_pressed(vk_up);
	option_pos = clamp(option_pos, 0, option_number-1);
		
	var _op_space = 40;
	var _op_bord = 4;
	for (var op = 0; op < option_number; op++)
		{
		var _o_w = string_width(option[op]) + _op_bord*2;
		draw_sprite_ext(txtb_spr, txtb_img, _txtb_x + 16, _txtb_y - _op_space*option_number + _op_space*op, _o_w/txtb_spr_w, (_op_space-1)/txtb_spr_h, 0, c_white, 1);
		
		if option_pos == op
			{
			draw_sprite(spr_arrow, 0, _txtb_x, _txtb_y - _op_space*option_number + _op_space*op);	
			}
		
		draw_text(_txtb_x + 16 + _op_bord, _txtb_y - _op_space*option_number + _op_space*op + 4, option[op]);
		}
	}

//draw text
var _drawtext = string_copy(text[page], 1, draw_char);
draw_text_ext(_txtb_x + border, _txtb_y + border+5, _drawtext, line_sep, line_width);
