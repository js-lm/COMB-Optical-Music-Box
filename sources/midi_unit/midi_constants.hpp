#pragma once

#include <cstdint>

namespace constants::midi{

    // UART configuration
    inline uart_inst_t *UartInstance{uart0};
    constexpr uint32_t BaudRate     {31250};
    constexpr uint8_t DataBits      {8};
    constexpr uint8_t StopBits      {1};

    // message status bytes
    constexpr uint8_t NoteOffStatus         {0x80};
    constexpr uint8_t NoteOnStatus          {0x90};
    constexpr uint8_t ProgramChangeStatus   {0xc0};
    constexpr uint8_t ControlChangeStatus   {0xb0};

    // message parameters
    constexpr uint8_t ChannelMask           {0x0f};
    constexpr uint8_t MaximumVelocity       {127};
    constexpr uint8_t MaximumNote           {127};
    constexpr uint16_t NumberOfNotes        {MaximumNote + 1};
    constexpr uint8_t MaximumChannel        {15};

    // controller
    constexpr uint8_t BankSelectController  {0};
    constexpr uint8_t AllSoundOffController {120};
    constexpr uint8_t ControllerValueOff    {0};

    // MT-32 sound variation
    constexpr uint8_t DefaultBankValue      {0};
    constexpr uint8_t AlternativeBankValue  {127};

    // drum sets
    constexpr uint8_t DrumSetsChannel{9};

} // namespace constants::midi
