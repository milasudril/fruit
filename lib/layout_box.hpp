#ifndef FRUIT_LAYOUTBOX_HPP
#define FRUIT_LAYOUTBOX_HPP

namespace fruit
{
	struct Dimensions
	{
		int min_width;
		int min_height;
		int max_width;
		int max_height;
	};

	class LayoutBox
	{
	public:
		template<class Widget>
		explicit LayoutBox(std::reference_wrapper<Widget> widget):m_obj{&widget.get()},
		m_get_dimension{[](void const* self){
			static_cast<Widget const*>(self)->get_dimensions();
		},
		m_get_dimension{[](void* self, Point<int> location, int width, int hegiht){
			static_cast<Widget*>(self)->get_dimensions(location, width, height);
		}}
		{}

		void const* object() const
		{
			return m_obj;
		}

		Dimensions get_dimensions() const
		{
			return m_get_dimension(m_obj);
		}

		void adjust_geometry(Point<int> location, int width, int hegiht)
		{
			m_adjust_geometry(m_obj, location, width, hegiht);
		}

	private:
		void* m_obj;

		Dimensions (*m_get_dimension)(void const* self);
		void (*m_adjust_geometry)(void* self, Point<int> location, int width, int hegiht);
	};
}

#endif