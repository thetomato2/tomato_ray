namespace tom
{

fn void process_keyboard_message(Button &new_state, const b32 is_down)
{
    if (new_state.ended_down != (is_down != 0)) {
        new_state.ended_down = is_down;
        ++new_state.half_transition_cnt;
    }
}

// NOTE: mod = Shift
fn char win32key_to_char_mod(Win32Key key)
{
    switch (key) {
        case space: return ' ';
        case d1: return '!';
        case d2: return '@';
        case d3: return '#';
        case d4: return '$';
        case d5: return '%';
        case d6: return '^';
        case d7: return '&';
        case d8: return '*';
        case d9: return '(';
        case d0: return ')';
        case a: return 'A';
        case b: return 'B';
        case c: return 'C';
        case d: return 'D';
        case e: return 'E';
        case f: return 'F';
        case g: return 'G';
        case h: return 'H';
        case i: return 'I';
        case j: return 'J';
        case k: return 'K';
        case l: return 'L';
        case m: return 'M';
        case n: return 'N';
        case o: return 'O';
        case p: return 'P';
        case q: return 'Q';
        case r: return 'R';
        case s: return 'S';
        case t: return 'T';
        case u: return 'U';
        case v: return 'V';
        case w: return 'W';
        case x: return 'X';
        case y: return 'Y';
        case z: return 'Z';
        case add: return '=';
        case enter: return '\n';
        case subtract: return '_';
        case semicolon: return ':';
        case comma: return '>';
        case period: return '<';
        case quotes: return '\"';
        case open_brackets: return '{';
        case close_brackets: return '}';
        case tilde: return '~';
        case backslash: return '|';
        case question: return '?';
    }

    return '\0';
}

fn char win32key_to_char(Win32Key key)
{
    switch (key) {
        case space: return ' ';
        case d1: return '1';
        case d2: return '2';
        case d3: return '3';
        case d4: return '4';
        case d5: return '5';
        case d6: return '6';
        case d7: return '7';
        case d8: return '8';
        case d9: return '9';
        case d0: return '0';
        case a: return 'a';
        case b: return 'b';
        case c: return 'c';
        case d: return 'd';
        case e: return 'e';
        case f: return 'f';
        case g: return 'g';
        case h: return 'h';
        case i: return 'i';
        case j: return 'j';
        case k: return 'k';
        case l: return 'l';
        case m: return 'm';
        case n: return 'n';
        case o: return 'o';
        case p: return 'p';
        case q: return 'q';
        case r: return 'r';
        case s: return 's';
        case t: return 't';
        case u: return 'u';
        case v: return 'v';
        case w: return 'w';
        case x: return 'x';
        case y: return 'y';
        case z: return 'z';
        case add: return '+';
        case enter: return '\n';
        case subtract: return '-';
        case semicolon: return ';';
        case comma: return ',';
        case period: return '.';
        case quotes: return '\'';
        case open_brackets: return '[';
        case close_brackets: return ']';
        case tilde: return '`';
        case backslash: return '\\';
        case question: return '/';
    }

    return '\0';
}

#define AssignWin32Key(key) result.keyboard.key.name = Win32Key::key;

fn Input init_input()
{
    Input result = {};

    AssignWin32Key(space);
    AssignWin32Key(d1);
    AssignWin32Key(d2);
    AssignWin32Key(d3);
    AssignWin32Key(d4);
    AssignWin32Key(d5);
    AssignWin32Key(d6);
    AssignWin32Key(d7);
    AssignWin32Key(d8);
    AssignWin32Key(d9);
    AssignWin32Key(d0);
    AssignWin32Key(a);
    AssignWin32Key(b);
    AssignWin32Key(c);
    AssignWin32Key(d);
    AssignWin32Key(e);
    AssignWin32Key(f);
    AssignWin32Key(g);
    AssignWin32Key(h);
    AssignWin32Key(i);
    AssignWin32Key(j);
    AssignWin32Key(k);
    AssignWin32Key(l);
    AssignWin32Key(m);
    AssignWin32Key(n);
    AssignWin32Key(o);
    AssignWin32Key(p);
    AssignWin32Key(q);
    AssignWin32Key(r);
    AssignWin32Key(s);
    AssignWin32Key(t);
    AssignWin32Key(u);
    AssignWin32Key(v);
    AssignWin32Key(w);
    AssignWin32Key(x);
    AssignWin32Key(y);
    AssignWin32Key(z);
    AssignWin32Key(enter);
    AssignWin32Key(escape);
    AssignWin32Key(left_alt);
    AssignWin32Key(left_shift);
    AssignWin32Key(left_control);
    AssignWin32Key(tab);
    AssignWin32Key(back);
    AssignWin32Key(add);
    AssignWin32Key(subtract);
    AssignWin32Key(semicolon);
    AssignWin32Key(comma);
    AssignWin32Key(period);
    AssignWin32Key(quotes);
    AssignWin32Key(open_brackets);
    AssignWin32Key(close_brackets);
    AssignWin32Key(tilde);
    AssignWin32Key(backslash);
    AssignWin32Key(question);
    AssignWin32Key(pipe);
    AssignWin32Key(left);
    AssignWin32Key(up);
    AssignWin32Key(right);
    AssignWin32Key(down);
    AssignWin32Key(f1);
    AssignWin32Key(f2);
    AssignWin32Key(f3);
    AssignWin32Key(f4);
    AssignWin32Key(f5);
    AssignWin32Key(f6);
    AssignWin32Key(f7);
    AssignWin32Key(f8);
    AssignWin32Key(f9);
    AssignWin32Key(f10);
    AssignWin32Key(f11);
    AssignWin32Key(f12);

    return result;
}

fn void do_input(Input *input, HWND hwnd, i32 ms_scroll)
{
    // mouse cursor
    POINT mouse_point;
    GetCursorPos(&mouse_point);
    ScreenToClient(hwnd, &mouse_point);
    input->mouse.pos_last.x = input->mouse.pos.x;
    input->mouse.pos_last.y = input->mouse.pos.y;
    input->mouse.pos.x      = (f32)mouse_point.x;
    input->mouse.pos.y      = (f32)mouse_point.y;

    input->mouse.scroll = ms_scroll;

    for (szt key = 0; key < CountOf(input->mouse.buttons); ++key) {
        if (input->mouse.buttons[key].half_transition_cnt > 0 &&
            input->mouse.buttons[key].ended_down == 0)
            input->mouse.buttons[key].half_transition_cnt = 0;
    }

    // mouse buttons
    process_keyboard_message(input->mouse.buttons[0], ::GetKeyState(VK_LBUTTON) & (1 << 15));
    process_keyboard_message(input->mouse.buttons[1], ::GetKeyState(VK_RBUTTON) & (1 << 15));
    process_keyboard_message(input->mouse.buttons[2], ::GetKeyState(VK_MBUTTON) & (1 << 15));

    for (szt key = 0; key < CountOf(input->keyboard.keys); ++key) {
        if (input->keyboard.keys[key].half_transition_cnt > 0 &&
            input->keyboard.keys[key].ended_down == 0)
            input->keyboard.keys[key].half_transition_cnt = 0;
    }

    for (u32 i = 0; i < Keyboard::key_cnt; ++i) {
        process_keyboard_message(input->keyboard.keys[i],
                                 ::GetKeyState(input->keyboard.keys[i].name) & (1 << 15));
    }
}

}  // namespace tom