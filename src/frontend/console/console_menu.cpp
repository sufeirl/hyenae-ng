/*
 * Hyenae NG
 *   Advanced Network Packet Generator (NextGen)
 *
 * Copyright (C) 2020 Robin Richter
 *
 *   Contact  : richterr@users.sourceforge.net
 *   Homepage : http://sourceforge.net/projects/hyenae-ng/
 *
 * This file is part of Hyenae NG.
 *
 * Hyenae NG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hyenae NG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hyenae NG.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../../include/assert.h"
#include "../../../include/frontend/console/console_menu.h"

#include <string>

namespace hyenae::frontend::console
{
    /*---------------------------------------------------------------------- */

    console_menu::console_menu(console_io* console_io, const string_t& title)
    {
        assert::argument_not_null(console_io, "console_io");

        _console_io = console_io;
        _title = title;
        _last_error = "";

    } /* console_menu */

    /*---------------------------------------------------------------------- */

    void console_menu::add_item(item* item)
    {
        _items.push_back(item);

    } /* add_option */

    /*---------------------------------------------------------------------- */

    console_menu::item* console_menu::prompt(
        item* default_choice, string_t error)
    {
        _console_io->header_out(_title);

        for (size_t pos = 0; pos < _items.size() - 1; pos++)
        {
            item_out(pos + 1, _items[pos]);
        }

        item_out(0, _items.back());

        if (error != "")
        {
            _console_io->error_out(error, true);
        }
        
        return choice_in(default_choice);

    } /* prompt */

    /*---------------------------------------------------------------------- */

    void console_menu::select_all(bool selected)
    {
        for (auto item : _items)
        {
            item->set_selected(selected);
        }

    } /* select_all */

    /*---------------------------------------------------------------------- */

    void console_menu::item_out(size_t pos, item* item)
    {
        _console_io->menu_item_out(
            pos, item->is_selected(),
            item->get_caption(),
            item->get_hint(),
            item->get_info(),
            _items.size());

    } /* item_out */

    /*---------------------------------------------------------------------- */

    console_menu::item* console_menu::choice_in(item* default_choice)
    {
        string_t input;
        string_t hint = "";
        size_t choice_pos = 0;
        size_t default_choice_pos = 0;

        if (_last_error != "")
        {
            _console_io->error_out(_last_error, true);
        }

        if (default_choice != NULL)
        {
            default_choice_pos = item_choice_pos(default_choice);
        }

        try
        {
            _last_error = "";

            hint.append("0-");
            hint.append(std::to_string(_items.size() - 1));

            if (default_choice != NULL)
            {
                hint.append(", Default = ");
                hint.append(std::to_string(default_choice_pos));
            }

            _console_io->separator_out(true, false);

            input = _console_io->prompt("Enter Selection", hint);

            if (input == "" && default_choice != NULL)
            {
                choice_pos = default_choice_pos;
            }
            else
            {
                choice_pos = std::stoi(input);

                assert::in_range(choice_pos < _items.size());
            }
            
            if (choice_pos == 0)
            {
                return _items.back();
            }
            else
            {
                return _items[choice_pos - 1];
            }
        }
        catch (const std::out_of_range&)
        {
            _last_error = concat(
                "Option out of range: ", input.c_str());
        }
        catch (const std::invalid_argument&)
        {
            _last_error = concat(
                "Invalid option: ", input.c_str());
        }

        return NULL;

    } /* choice_in */

    /*---------------------------------------------------------------------- */

    size_t console_menu::item_choice_pos(item* item)
    {
        for (size_t pos = 0; pos < _items.size(); pos++)
        {
            if (_items[pos] == item)
            {
                return pos < _items.size() - 1 ? pos + 1 : 0;
            }
        }

        assert::legal_state(false, "", "item could not be found");

        return (size_t)-1;

    } /* item_choice_pos */

    /*---------------------------------------------------------------------- */

} /* hyenae::frontend::console */