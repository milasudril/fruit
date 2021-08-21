//@	{"dependencies_extra":[{"ref":"./line_layout.o", "rel":"implementation"}]}

#ifndef FRUIT_COLUMNLAYOUT_HPP
#define FRUIT_COLUMNLAYOUT_HPP

#include "./layout_manager.hpp"
#include "./error_message.hpp"

#include <vector>
#include <variant>

namespace fruit
{
	/**
	 * \brief Positions its member in a straight line
	 *
	 * A LineLayout positions its member in a straight line. A LinieLayout can organize its member
	 * either horizontally or vertically. Repositioning is triggered as a response to a
	 * GeometryUpdateEvent.
	 *
	*/
	class LineLayout
	{
	public:
		struct Minimize{};

		/**
		 * \brief Determines the layout direction
		 *
		 * The direction determines the positioning scheme of the LineLayout
		 *
		*/
		enum class Direction:int
		{
			LeftToRight,  /**< Use left-to-right or horizontal positioning scheme*/
			TopToBottom   /**< Use top-to-bottom or vertical positioning scheme*/
		};

		/**
		 * \brief Constructs a new LineLayout, and sets the direction to dir
		*/
		explicit LineLayout(Direction dir = Direction::LeftToRight):
			m_direction{dir},
			m_min_width{Minimize{}},
			m_min_height{Minimize{}}
		{}

		void push_back(LayoutBox const& box)
		{
			m_content.push_back(box);
		}

		SizeRequestResult handle(SizeRequestEvent const&) const;

		void handle(GeometryUpdateEvent const& event);

		void set_direction(Direction dir)
		{
			m_direction = dir;
		}

		Direction get_direction() const
		{
			return m_direction;
		}

		size_t widget_count() const { return std::size(m_content); }

		template<class T>
		void set_width(T&& value)
		{  m_min_width = std::forward<T>(value);}

		template<class T>
		void set_height(T&& value)
		{ m_min_height = std::forward<T>(value);}

		ViewportSize compute_min_size(ViewportSize domain_size) const;

	private:
		Direction m_direction;
		std::vector<LayoutBox> m_content;
		std::variant<Minimize, int, float> m_min_width;
		std::variant<Minimize, int, float> m_min_height;
	};
}

#endif