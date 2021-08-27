//@	{"dependencies_extra":[{"ref":"./line_layout.o", "rel":"implementation"}]}

#ifndef FRUIT_COLUMNLAYOUT_HPP
#define FRUIT_COLUMNLAYOUT_HPP

#include "./layout_manager.hpp"
#include "./error_message.hpp"
#include "./elastic_viewport_size.hpp"

#include <vector>
#include <variant>

namespace fruit
{
	/**
	 * \brief Positions its member in a straight line
	 *
	 * A LineLayout locations its member in a straight line. A LinieLayout can organize its member
	 * either horizontally or vertically. Relocationing is triggered as a response to a
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
		 * The direction determines the location scheme of the LineLayout
		 *
		*/
		enum class Direction:int
		{
			LeftToRight,  /**< Use left-to-right or horizontal location scheme*/
			TopToBottom   /**< Use top-to-bottom or vertical location scheme*/
		};

		/**
		 * \brief Constructs a new LineLayout, and sets the direction to dir
		 *
		 * This constructor sets the dierection to dir.
		*/
		explicit LineLayout(Direction dir = Direction::LeftToRight):
			m_direction{dir},
			m_min_width{Minimize{}},
			m_min_height{Minimize{}}
		{}

		/**
		 * \brief Places box at the end of the line
		 *
		 * This function places box at the end of the line.
		*/
		void push_back(LayoutBox const& box)
		{
			m_content.push_back(box);
		}

		/**
		 * \brief Wrapper function that exists so that a LineLayout can respond to
		 *        \ref SizeRequestEvent "SizeRequestEvents"
		 *
		 * This function is wrapper function that exists so that LineLayout can respond to
		 * \ref SizeRequestEvent "SizeRequestEvents". This makes it possible to nest LineLayout
		 * objects.
		 *
		 * \return a SizeRequestResult where min_size is the return value of compute_min_size(ViewportSize) const
		 *
		 * \see compute_min_size
		*/
		SizeRequestResult handle(SizeRequestEvent const& event) const
		{
			auto const min_size = compute_min_size(event.domain_size);
			return SizeRequestResult{min_size, min_size};
		}

		/**
		 * \brief Processes \ref GeometryUpdateEvent "GeometryUpdateEvents"
		 *
		 * This function will update the location and size of all members of this LineLayout, given
		 * min_width, min_height, and the ViewportSize specified in event. Also, the constraints
		 * givien by \ref LayoutBox "LayoutBoxes" within this LineLayout.
		 *
		 * \todo Describe algorithm
		*/
		void handle(GeometryUpdateEvent const& event);

		/**
		 * \brief Sets the Direction of this LineLayout
		 *
		 * This function sets the direction of this LineLayout.
		*/
		void set_direction(Direction dir)
		{
			m_direction = dir;
		}

		/**
		 * \brief Retrieves the current Direction of this LineLayout
		 *
		 * This function retrieves the direction of this LineLayout.
		 *
		 * \return The Direction of this LineLayout
		*/
		Direction direction() const
		{
			return m_direction;
		}

		/**
		 * \brief Retrieves the number of members in this LineLayout
		 *
		 * This function retrieves the number of members in this LineLayout
		 *
		 * \return The number of members in this LineLayout
		*/
		size_t widget_count() const { return std::size(m_content); }

		/**
		 * \brief Sets the width of this LineLayout, relative to the size passed to
		 * compute_min_size(ViewportSize) const
		 *
		 * This function sets the width of this LineLayout, relative to the size passed to
		 * compute_min_size(ViewportSize) const.
		 *
		 * \note The actual width may be larger, if required by any of the members
		 */
		void set_width(float value)
		{  m_min_width = value;}

		/**
		 * \brief Sets the width in pixels of this LineLayout
		 *
		 * This function sets the width in pixels of this LineLayout.
		 *
		 * \note The actual width may be larger, if required by any of the members
		 */
		void set_width(int value)
		{  m_min_width = value;}

		/**
		 * \brief Sets the height of this LineLayout, relative to the size passed to
		 * compute_min_size(ViewportSize) const
		 *
		 * This function sets the height of this LineLayout, relative to the size passed to
		 * compute_min_size(ViewportSize) const.
		 *
		 * \note The actual height may be larger, if required by any of the members
		 */
		void set_height(float value)
		{ m_min_height = value;}

		/**
		 * \brief Sets the height in pixels of this LineLayout
		 *
		 * This function sets the height in pixels of this LineLayout.
		 *
		 * \note The actual height may be larger, if required by any of the members
		 */
		void set_height(int value)
		{ m_min_height = value;}

		/**
		 * \brief Computes the minimum space that this LineLayout will require, given domain_size
		 *
		 * Given domain_size, this funciton computes the minimum space this LineLayout will require.
		 * In addition to domain_size, the space requried depends on min_width, min_height, and the
		 * space requirements for the members.
		 *
		 * The size is computed using the following algorithm:
		 *
		 * 1. Let s be the required ViewportSize, initialized to {0, 0}
		 * 2.  For each member:
		 *   1. Let s' = min_size returned by member . SizeRequestEvent{domain_size}
		 *   2. Let s_req = max(s', requested_size(member, domain_size))
		 *   3. If
		 *     1. m_direction equals LeftToRight:
		 *        1. let s.width = s.width + s_req.width
		 *        2. let s.height = max(s.height, s_req).height
		 *     2. otherwise:
		 *        1. let s.width = max(s.width, s_req).width
		 *        2. let s.height = s.height + s_req.height
		 * 3. Return max(s, requested_size(*this, domain_size))
		 */
		ViewportSize compute_min_size(ViewportSize domain_size) const;

		ElasticViewportSize min_size() const
		{ return m_min_size; }

	private:
		Direction m_direction;
		std::vector<LayoutBox> m_content;
		ElasticViewportSize m_min_size;
	};

	ViewportSize requested_size(LineLayout const& layout, ViewportSize domain_size)
	{ return layout.min_size().viewportSize(domain_size); }
}

#endif