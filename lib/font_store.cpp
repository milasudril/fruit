//@	{
//@	"target":{"name":"font_store.o"}
//@	}

#include "./font_store.hpp"

fruit::FontResource fruit::FontStore::load_and_replace(std::vector<std::byte>&& buffer)
{
	fruit::FontFace face{m_loader,std::move(buffer)};

	auto name = std::string{face.family()};
	name.append("/").append(face.style());

	auto ip = m_fonts.insert_or_assign(std::move(name), std::move(face));

	return FontResource{ip.first->first, &ip.first->second};
}

