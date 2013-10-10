
class_<Vec3>("Vec3")
	.def_readwrite("x", &Vec3::x)
	.def_readwrite("y", &Vec3::y)
	.def_readwrite("z", &Vec3::z)
	.def(init<>())
	.def(init<const Vec3&>())
	.def(init<float, float, float>())
	.def(init<float>())
	.def(init<const Vec4&>())
	.def("set", &Vec3::set)
;
