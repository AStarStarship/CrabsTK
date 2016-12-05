# _2D API
API for 2D math and geometry. In order to be able to operate on data across an assembly boundary, there are no templates in Kabuki Software Development Kit. This is more of a hassle to develop, but become very eloquent when used with typedefs or C++11 aliases. The Kabuki Project uses Hungarian notation to denote the underlying data type.

### Examples
```
using _2D::Point = _2D::Point_f;        //< C++11 alias can be replaced with "typedef Point_f Point;".
static _2D::Point P = { 0.0f, 0.0f };   //< The alias or typedef makes the hungarian notation go away!
```

## License ##
Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                    All right reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.