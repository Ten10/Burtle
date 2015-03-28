#pragma once

struct non_copyable
{
	non_copyable(const non_copyable& Other) = delete;
	non_copyable() = default;
};