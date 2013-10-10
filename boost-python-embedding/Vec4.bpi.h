
class_<Vec4::Nested>("Nested")
	.def("foo", &Vec4::Nested::foo)
;

class_<Vec4>("Vec4")
	.def_readwrite("nested", &Vec4::nested)
	.def_readwrite("x", &Vec4::x)
	.def_readwrite("y", &Vec4::y)
	.def_readwrite("z", &Vec4::z)
	.def_readwrite("w", &Vec4::w)
	.def(init<const Vec4&>())
	.def(init<float, float, float, float>())
	.def(init<float>())
	.def(init<const Vec3&, float>())
;
