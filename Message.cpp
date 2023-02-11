#include <filesystem>
#include "Message.h"

Message::Message(std::string const& _sender, std::string const& _recipient, std::string const& _message) : sender(_sender), recipient(_recipient), message(_message) {}

std::string Message::getSender()	const noexcept { return sender; }
std::string Message::getRecipient() const noexcept { return recipient; }
std::string Message::getMessage()	const noexcept { return message; }