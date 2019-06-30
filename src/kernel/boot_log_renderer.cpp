#include <kernel/display/termcolor.h>
#include <kernel/log.h>
#include <stdio.h>
#include <string.h>

void BootLogRenderer::Init() {
    log_tag_width = 10;
    log_scroll_current = 0;
}

void BootLogRenderer::SignalNewEntry() {
    log_scroll_current = g_log.log_entry_index - g_log.impl->height;

    if (log_scroll_current < 0) {
        log_scroll_current = 0;
    }

    if (LOG_MAX_ENTRIES < g_log.impl->height) {
        // Do nothing, because we can fit every entry on screen.
    } else if (log_scroll_current > (int)(LOG_MAX_ENTRIES - g_log.impl->height)) {
        log_scroll_current = LOG_MAX_ENTRIES - g_log.impl->height;
        scrollAppend = true;
    }

    UpdateRender();
}

void BootLogRenderer::UpdateRender() {
    // Determine what kind of change happened
    // There are 4 kinds of changes
    // Append (Nothing is scrolled. Usually for the first few boot messages)
    // Scroll up (new entry added)
    // Scroll down (user attempts to scroll to view previous messages)
    // Full Render (Last resort)
    RenderState currentState = Append;

    if (requestFullRender == true) {
        currentState = Full;
        requestFullRender = false;
    } else if (log_scroll_current == 0 && log_scroll_previous == 0) {
        currentState = Append;
    } else if (log_scroll_current >= log_scroll_previous) {
        currentState = ScrollUp;
    } else if (log_scroll_current == (int)(LOG_MAX_ENTRIES - g_log.impl->height)) {
        currentState = ScrollUp;
    } else if (log_scroll_current < log_scroll_previous) {
        currentState = ScrollDown;
    }

    if (requestFullRender == true) {
        currentState = Full;
        requestFullRender = false;
    }

    // Serial devices handle their own scrolling, so we only append data to them.
    if (g_log.impl->isSerial) {
        currentState = Append;
    }

    switch (currentState) {
        case RenderState::Append:
            StateAppend();
            break;
        case RenderState::ScrollDown:
            StateScrollDown();
            break;
        case RenderState::ScrollUp:
            StateScrollUp();
            break;
        default:
        case RenderState::Full:
            StateFull();
            break;
    }
    log_scroll_previous = log_scroll_current;
}

void BootLogRenderer::PrintEntry(LogEntry entry) {
    //LogColor oldColor = g_log.currentColor;
    LogColor toSet = g_log.GetSeverityColor(entry.severity);

    // Set to defaults...
    if (toSet.fore == TERMINAL_TRANSPARENT) {
        toSet.fore = g_log.defaultColor.fore;
    }

    if (toSet.back == TERMINAL_TRANSPARENT) {
        toSet.back = g_log.defaultColor.back;
    }

    g_log.SetColors(toSet);

    // Print the message to the terminal.

    PrintTag(entry.tag);
    PrintMessage(entry.message);
    g_log.impl->Print('\n');

    // Set color again
    //g_log.SetColors(oldColor);
}

void BootLogRenderer::PrintTag(const char* tag) {
    // How much padding between tag and layout characters
    const int padding_size = 2;

    int tag_len = strlen(tag);
    int tag_padding_len = tag_len + padding_size;

    // If tag + pad length is bigger then what we've reserved, expand.
    if (tag_padding_len > log_tag_width) {
        log_tag_width = tag_padding_len;
    }

    // Print right justified
    int right_space_width = log_tag_width - tag_padding_len;
    for (int i = 0; i < right_space_width; i++) {
        g_log.impl->Print(' ');
    }
    printf("%s: ", tag);
}

void BootLogRenderer::PrintMessage(const char* str) {
    // Set left margin...
    g_log.impl->left_margin = log_tag_width;
    // Print
    printf("%s", str);
    // Reset left margin
    g_log.impl->left_margin = 0;
}

void BootLogRenderer::StateFull() {
    // Set the background color so we clear to the correct color
    // TODO: Should be default color.
    LogColor oldColor = g_log.currentColor;
    g_log.SetBackgroundColor(0);
    g_log.impl->Clear();
    // Set the background color back
    g_log.SetBackgroundColor(oldColor.back);

    size_t max_scroll = g_log.impl->height + log_scroll_current;
    if (max_scroll > LOG_MAX_ENTRIES - 1) {
        max_scroll = LOG_MAX_ENTRIES - 1;
    }

    // Render entries to the terminal.
    for (size_t i = log_scroll_current; i < max_scroll; i++) {
        if (i >= g_log.log_entry_index) {
            break;
        }
        PrintEntry(g_log.entry_buffer[i]);
    }
}

void BootLogRenderer::StateAppend() {
    // Just print the last entry...
    PrintEntry(g_log.entry_buffer[g_log.log_entry_index - 1]);
}

void BootLogRenderer::StateScrollDown() {}

void BootLogRenderer::StateScrollUp() {
    int amountToScroll = log_scroll_current - log_scroll_previous;

    // We must scroll down at least one line
    // if we're in the scroll up state
    if (amountToScroll < 1) {
        amountToScroll = 1;
    }
    // Scroll and print entry...
    g_log.impl->Scroll(amountToScroll);
    PrintEntry(g_log.entry_buffer[g_log.log_entry_index - 1]);
}

void BootLogRenderer::SignalFullRender() {
    requestFullRender = true;
}
