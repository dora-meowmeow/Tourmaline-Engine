<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.14.0">
  <compound kind="file">
    <name>Concepts.hpp</name>
    <path>headers/</path>
    <filename>Concepts_8hpp.html</filename>
    <class kind="struct">Tourmaline::Concepts::FunctionTraits&lt; Return(*)(Arguments...)&gt;</class>
    <concept>Tourmaline::Concepts::Hashable</concept>
    <concept>Tourmaline::Concepts::Either</concept>
  </compound>
  <compound kind="file">
    <name>ContainerOptions.hpp</name>
    <path>headers/Containers/</path>
    <filename>ContainerOptions_8hpp.html</filename>
    <class kind="struct">Tourmaline::Containers::HashContainerOptions</class>
    <class kind="struct">Tourmaline::Containers::DualKeyMapOptions</class>
  </compound>
  <compound kind="file">
    <name>DualkeyMap.hpp</name>
    <path>headers/Containers/</path>
    <filename>DualkeyMap_8hpp.html</filename>
    <includes id="Concepts_8hpp" name="Concepts.hpp" local="yes" import="no" module="no" objc="no">../Concepts.hpp</includes>
    <includes id="Logging_8hpp" name="Logging.hpp" local="yes" import="no" module="no" objc="no">../Systems/Logging.hpp</includes>
    <includes id="ContainerOptions_8hpp" name="ContainerOptions.hpp" local="yes" import="no" module="no" objc="no">ContainerOptions.hpp</includes>
    <includes id="Hashmap_8hpp" name="Hashmap.hpp" local="yes" import="no" module="no" objc="no">Hashmap.hpp</includes>
    <class kind="class">Tourmaline::Containers::DualkeyMap</class>
    <class kind="struct">Tourmaline::Containers::DualkeyMap::MultiQueryResult</class>
  </compound>
  <compound kind="file">
    <name>Hashlist.hpp</name>
    <path>headers/Containers/</path>
    <filename>Hashlist_8hpp.html</filename>
    <includes id="Concepts_8hpp" name="Concepts.hpp" local="yes" import="no" module="no" objc="no">../Concepts.hpp</includes>
    <includes id="Logging_8hpp" name="Logging.hpp" local="yes" import="no" module="no" objc="no">../Systems/Logging.hpp</includes>
    <includes id="ContainerOptions_8hpp" name="ContainerOptions.hpp" local="yes" import="no" module="no" objc="no">ContainerOptions.hpp</includes>
    <class kind="class">Tourmaline::Containers::Hashlist</class>
  </compound>
  <compound kind="file">
    <name>Hashmap.hpp</name>
    <path>headers/Containers/</path>
    <filename>Hashmap_8hpp.html</filename>
    <includes id="Concepts_8hpp" name="Concepts.hpp" local="yes" import="no" module="no" objc="no">../Concepts.hpp</includes>
    <includes id="Logging_8hpp" name="Logging.hpp" local="yes" import="no" module="no" objc="no">../Systems/Logging.hpp</includes>
    <includes id="ContainerOptions_8hpp" name="ContainerOptions.hpp" local="yes" import="no" module="no" objc="no">ContainerOptions.hpp</includes>
    <class kind="class">Tourmaline::Containers::Hashmap</class>
  </compound>
  <compound kind="file">
    <name>Input.hpp</name>
    <path>headers/Game/</path>
    <filename>Input_8hpp.html</filename>
    <includes id="Keyboard_8hpp" name="Keyboard.hpp" local="yes" import="no" module="no" objc="no">Input/Keyboard.hpp</includes>
    <includes id="Pointer_8hpp" name="Pointer.hpp" local="yes" import="no" module="no" objc="no">Input/Pointer.hpp</includes>
  </compound>
  <compound kind="file">
    <name>Axis.hpp</name>
    <path>headers/Game/Input/</path>
    <filename>Axis_8hpp.html</filename>
    <includes id="Keyboard_8hpp" name="Keyboard.hpp" local="yes" import="no" module="no" objc="no">Keyboard.hpp</includes>
  </compound>
  <compound kind="file">
    <name>Keyboard.hpp</name>
    <path>headers/Game/Input/</path>
    <filename>Keyboard_8hpp.html</filename>
    <class kind="class">Tourmaline::Game::Input::Key</class>
  </compound>
  <compound kind="file">
    <name>Pointer.hpp</name>
    <path>headers/Game/Input/</path>
    <filename>Pointer_8hpp.html</filename>
    <includes id="Hashmap_8hpp" name="Hashmap.hpp" local="yes" import="no" module="no" objc="no">../../Containers/Hashmap.hpp</includes>
    <class kind="struct">Tourmaline::Game::Input::Pointer</class>
  </compound>
  <compound kind="file">
    <name>Program.hpp</name>
    <path>headers/Game/</path>
    <filename>Program_8hpp.html</filename>
    <includes id="Hashmap_8hpp" name="Hashmap.hpp" local="yes" import="no" module="no" objc="no">../Containers/Hashmap.hpp</includes>
    <includes id="ECS_8hpp" name="ECS.hpp" local="yes" import="no" module="no" objc="no">../Systems/ECS.hpp</includes>
    <includes id="Input_8hpp" name="Input.hpp" local="yes" import="no" module="no" objc="no">Input.hpp</includes>
    <class kind="class">Tourmaline::Game::Program</class>
    <class kind="struct">Tourmaline::Game::Program::Config</class>
    <class kind="struct">Tourmaline::Game::Program::Args</class>
  </compound>
  <compound kind="file">
    <name>ECS.hpp</name>
    <path>headers/Systems/</path>
    <filename>ECS_8hpp.html</filename>
    <includes id="Concepts_8hpp" name="Concepts.hpp" local="yes" import="no" module="no" objc="no">../Concepts.hpp</includes>
    <includes id="DualkeyMap_8hpp" name="DualkeyMap.hpp" local="yes" import="no" module="no" objc="no">../Containers/DualkeyMap.hpp</includes>
    <includes id="Hashlist_8hpp" name="Hashlist.hpp" local="yes" import="no" module="no" objc="no">../Containers/Hashlist.hpp</includes>
    <includes id="Hashmap_8hpp" name="Hashmap.hpp" local="yes" import="no" module="no" objc="no">../Containers/Hashmap.hpp</includes>
    <includes id="UUID_8hpp" name="UUID.hpp" local="yes" import="no" module="no" objc="no">../Types/UUID.hpp</includes>
    <includes id="UnspecifiedType_8hpp" name="UnspecifiedType.hpp" local="yes" import="no" module="no" objc="no">../Types/UnspecifiedType.hpp</includes>
    <includes id="BuiltinComponents_8hpp" name="BuiltinComponents.hpp" local="yes" import="no" module="no" objc="no">ECS/BuiltinComponents.hpp</includes>
    <includes id="Prefab_8hpp" name="Prefab.hpp" local="yes" import="no" module="no" objc="no">ECS/Prefab.hpp</includes>
    <includes id="Logging_8hpp" name="Logging.hpp" local="yes" import="no" module="no" objc="no">Logging.hpp</includes>
    <includes id="Random_8hpp" name="Random.hpp" local="yes" import="no" module="no" objc="no">Random.hpp</includes>
    <class kind="class">Tourmaline::Systems::ECS::World</class>
  </compound>
  <compound kind="file">
    <name>BuiltinComponents.hpp</name>
    <path>headers/Systems/ECS/</path>
    <filename>BuiltinComponents_8hpp.html</filename>
    <includes id="Matrix_8hpp" name="Matrix.hpp" local="yes" import="no" module="no" objc="no">../../Types/Matrix.hpp</includes>
    <class kind="struct">Tourmaline::Systems::ECS::Component</class>
    <class kind="struct">Tourmaline::Systems::Components::Transform</class>
    <concept>Tourmaline::Systems::ECS::isAComponent</concept>
  </compound>
  <compound kind="file">
    <name>Prefab.hpp</name>
    <path>headers/Systems/ECS/</path>
    <filename>Prefab_8hpp.html</filename>
    <includes id="BuiltinComponents_8hpp" name="BuiltinComponents.hpp" local="yes" import="no" module="no" objc="no">BuiltinComponents.hpp</includes>
    <class kind="class">Tourmaline::Systems::ECS::Prefab</class>
  </compound>
  <compound kind="file">
    <name>Importer.hpp</name>
    <path>headers/Systems/</path>
    <filename>Importer_8hpp.html</filename>
    <class kind="class">Tourmaline::Systems::Importer</class>
    <class kind="struct">Tourmaline::Systems::Importer::MeshInfo</class>
  </compound>
  <compound kind="file">
    <name>Logging.hpp</name>
    <path>headers/Systems/</path>
    <filename>Logging_8hpp.html</filename>
    <class kind="class">Tourmaline::Systems::Logging</class>
  </compound>
  <compound kind="file">
    <name>Random.hpp</name>
    <path>headers/Systems/</path>
    <filename>Random_8hpp.html</filename>
    <includes id="UUID_8hpp" name="UUID.hpp" local="yes" import="no" module="no" objc="no">../Types/UUID.hpp</includes>
    <class kind="class">Tourmaline::Systems::Random</class>
  </compound>
  <compound kind="file">
    <name>Serialization.hpp</name>
    <path>headers/Systems/</path>
    <filename>Serialization_8hpp.html</filename>
    <includes id="DataHandling_8hpp" name="DataHandling.hpp" local="yes" import="no" module="no" objc="no">Serialization/DataHandling.hpp</includes>
    <class kind="struct">Tourmaline::Systems::Serialization::Serializable</class>
    <class kind="struct">Tourmaline::Systems::Serialization::Serializable::SerialList</class>
  </compound>
  <compound kind="file">
    <name>DataHandling.hpp</name>
    <path>headers/Systems/Serialization/</path>
    <filename>DataHandling_8hpp.html</filename>
  </compound>
  <compound kind="file">
    <name>Matrix.hpp</name>
    <path>headers/Types/</path>
    <filename>Matrix_8hpp.html</filename>
    <includes id="Units_8hpp" name="Units.hpp" local="yes" import="no" module="no" objc="no">Units.hpp</includes>
    <class kind="class">Tourmaline::Type::Matrix</class>
  </compound>
  <compound kind="file">
    <name>Units.hpp</name>
    <path>headers/Types/</path>
    <filename>Units_8hpp.html</filename>
  </compound>
  <compound kind="file">
    <name>UnspecifiedType.hpp</name>
    <path>headers/Types/</path>
    <filename>UnspecifiedType_8hpp.html</filename>
    <class kind="struct">Tourmaline::Type::UnspecifiedType</class>
  </compound>
  <compound kind="file">
    <name>UUID.hpp</name>
    <path>headers/Types/</path>
    <filename>UUID_8hpp.html</filename>
    <class kind="class">Tourmaline::Type::UUID</class>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Game::Program::Args</name>
    <filename>structTourmaline_1_1Game_1_1Program_1_1Args.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>argc</name>
      <anchorfile>structTourmaline_1_1Game_1_1Program_1_1Args.html</anchorfile>
      <anchor>a3710ea168680881b66d81477eba4d2cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char **</type>
      <name>argv</name>
      <anchorfile>structTourmaline_1_1Game_1_1Program_1_1Args.html</anchorfile>
      <anchor>a5eb7eff49d9a26d93315df587beaa682</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Shaders::Base</name>
    <filename>classTourmaline_1_1Shaders_1_1Base.html</filename>
    <base>Magnum::GL::AbstractShaderProgram</base>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Systems::ECS::Component</name>
    <filename>structTourmaline_1_1Systems_1_1ECS_1_1Component.html</filename>
    <member kind="function">
      <type></type>
      <name>Component</name>
      <anchorfile>structTourmaline_1_1Systems_1_1ECS_1_1Component.html</anchorfile>
      <anchor>a6913d37a4438ac99ea1e72496cbbeeae</anchor>
      <arglist>(bool enabled=true)</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isEnabled</name>
      <anchorfile>structTourmaline_1_1Systems_1_1ECS_1_1Component.html</anchorfile>
      <anchor>a5658acfae2768974cd8f797257df2c4c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Game::Program::Config</name>
    <filename>structTourmaline_1_1Game_1_1Program_1_1Config.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>WindowMode</name>
      <anchorfile>structTourmaline_1_1Game_1_1Program_1_1Config.html</anchorfile>
      <anchor>ab5dd74a7192444f06fea3d5565707091</anchor>
      <arglist></arglist>
      <enumvalue file="structTourmaline_1_1Game_1_1Program_1_1Config.html" anchor="ab5dd74a7192444f06fea3d5565707091a0829ea6734059d66e6bf87096b215dc1">Fullscreen</enumvalue>
      <enumvalue file="structTourmaline_1_1Game_1_1Program_1_1Config.html" anchor="ab5dd74a7192444f06fea3d5565707091a78f0192ac55eba33ee88d026452952e5">Borderless</enumvalue>
      <enumvalue file="structTourmaline_1_1Game_1_1Program_1_1Config.html" anchor="ab5dd74a7192444f06fea3d5565707091a49d903a5c02560cf79bf6b516cc89457">Maximized</enumvalue>
      <enumvalue file="structTourmaline_1_1Game_1_1Program_1_1Config.html" anchor="ab5dd74a7192444f06fea3d5565707091ac1999bb36fd75c30832ec398d5b249ea">BorderlessFullscreen</enumvalue>
      <enumvalue file="structTourmaline_1_1Game_1_1Program_1_1Config.html" anchor="ab5dd74a7192444f06fea3d5565707091ab13311ab51c4c34757f67f26580018dd">Windowed</enumvalue>
    </member>
    <member kind="variable">
      <type>Corrade::Containers::String</type>
      <name>windowTitle</name>
      <anchorfile>structTourmaline_1_1Game_1_1Program_1_1Config.html</anchorfile>
      <anchor>abd7c6507ce652faf7d0b10026db324aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Magnum::Vector2i</type>
      <name>windowSize</name>
      <anchorfile>structTourmaline_1_1Game_1_1Program_1_1Config.html</anchorfile>
      <anchor>a9bb0d42a1319e03e681af811c1a4eaa8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>desiredFrameRate</name>
      <anchorfile>structTourmaline_1_1Game_1_1Program_1_1Config.html</anchorfile>
      <anchor>a80e3c2e2cb5f9afd9c97d83913cff15d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>WindowMode</type>
      <name>windowMode</name>
      <anchorfile>structTourmaline_1_1Game_1_1Program_1_1Config.html</anchorfile>
      <anchor>a5da532e7db88c68e9061c0620f3cb2be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>vsyncEnabled</name>
      <anchorfile>structTourmaline_1_1Game_1_1Program_1_1Config.html</anchorfile>
      <anchor>a738237bd97353948bd6a144b86fb8737</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Containers::DualkeyMap</name>
    <filename>classTourmaline_1_1Containers_1_1DualkeyMap.html</filename>
    <templarg>Concepts::Hashable AKey</templarg>
    <templarg>Concepts::Hashable BKey</templarg>
    <templarg>typename Value</templarg>
    <class kind="struct">Tourmaline::Containers::DualkeyMap::MultiQueryResult</class>
    <member kind="typedef">
      <type>std::pair&lt; std::variant&lt; std::monostate, std::reference_wrapper&lt; const AKey &gt;, std::reference_wrapper&lt; const BKey &gt; &gt;, Value &amp; &gt;</type>
      <name>QueryResult</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>a80ed35da8913f36e55982658c651f8a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::tuple&lt; const AKey &amp;, const BKey &amp;, Value &amp; &gt;</type>
      <name>Entry</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>a57c2fecbecb303017e889899dd016820</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DualkeyMap</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>a0daf1c42fa0894fe188b508373b39363</anchor>
      <arglist>(const DualkeyMap &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>DualkeyMap &amp;</type>
      <name>operator=</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>a725ce15dfbce7f937dcc369208f6764c</anchor>
      <arglist>(const DualkeyMap &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Entry</type>
      <name>Insert</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>a96507dfbd643ff741d175155efbde128</anchor>
      <arglist>(AKey firstKey, BKey secondKey, Value value)</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>Remove</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>a8c85831bdecd2bd3d183295546525192</anchor>
      <arglist>(std::optional&lt; AKey &gt; firstKey, std::optional&lt; BKey &gt; secondKey)</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>Count</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>aa27700a8b1de5544667752755f19379b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Clear</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>ac7e2fc3a3c636815388c3d8cfc33075f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; QueryResult &gt;</type>
      <name>Query</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>af248f2fcfcb8741f3b392597946acf63</anchor>
      <arglist>(std::optional&lt; AKey &gt; firstKey, std::optional&lt; BKey &gt; secondKey)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; MultiQueryResult&lt; OppositeKey &gt; &gt;</type>
      <name>QueryWithAll</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>a7c0cad3be727479167752231d6df2e63</anchor>
      <arglist>(Corrade::Containers::ArrayView&lt; Key &gt; keys, bool ignoreChecks=false)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Scan</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>a04091944f1347d5e7d1ff842ebbafcd3</anchor>
      <arglist>(std::function&lt; bool(const std::size_t firstKeyHash, const std::size_t secondKeyHash, Value &amp;value)&gt; scanFunction)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Scan</name>
      <anchorfile>classTourmaline_1_1Containers_1_1DualkeyMap.html</anchorfile>
      <anchor>ad6f07626d9689bf696804862e9e531b0</anchor>
      <arglist>(std::function&lt; bool(const AKey &amp;firstKey, const BKey &amp;secondKey, Value &amp;value)&gt; scanFunction)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Containers::DualKeyMapOptions</name>
    <filename>structTourmaline_1_1Containers_1_1DualKeyMapOptions.html</filename>
    <member kind="variable">
      <type>std::uint64_t</type>
      <name>baseReservation</name>
      <anchorfile>structTourmaline_1_1Containers_1_1DualKeyMapOptions.html</anchorfile>
      <anchor>a4c9ae594e30a824c40e28eb52179de35</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Concepts::FunctionTraits&lt; Return(*)(Arguments...)&gt;</name>
    <filename>structTourmaline_1_1Concepts_1_1FunctionTraits_3_01Return_07_5_08_07Arguments_8_8_8_08_4.html</filename>
    <templarg>typename Return</templarg>
    <templarg>typename... Arguments</templarg>
    <member kind="typedef">
      <type>Return</type>
      <name>returnType</name>
      <anchorfile>structTourmaline_1_1Concepts_1_1FunctionTraits_3_01Return_07_5_08_07Arguments_8_8_8_08_4.html</anchorfile>
      <anchor>ae05cd242901ebd600633c8e3048b576c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::tuple&lt; Arguments... &gt;</type>
      <name>arguments</name>
      <anchorfile>structTourmaline_1_1Concepts_1_1FunctionTraits_3_01Return_07_5_08_07Arguments_8_8_8_08_4.html</anchorfile>
      <anchor>ae2c65263eb182db0dd889d5488ad71e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::tuple_element_t&lt; index, arguments &gt;</type>
      <name>argument</name>
      <anchorfile>structTourmaline_1_1Concepts_1_1FunctionTraits_3_01Return_07_5_08_07Arguments_8_8_8_08_4.html</anchorfile>
      <anchor>a9a7112e86d868a4a2be6d5b0d5d7bc89</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr std::size_t</type>
      <name>argumentCount</name>
      <anchorfile>structTourmaline_1_1Concepts_1_1FunctionTraits_3_01Return_07_5_08_07Arguments_8_8_8_08_4.html</anchorfile>
      <anchor>a2e60f6549022298ce5f1a311932f0d67</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Containers::HashContainerOptions</name>
    <filename>structTourmaline_1_1Containers_1_1HashContainerOptions.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>loadFactor</name>
      <anchorfile>structTourmaline_1_1Containers_1_1HashContainerOptions.html</anchorfile>
      <anchor>a18bf02db01f0ad9c8bb96a6d9a0b1f4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>minimizeFactor</name>
      <anchorfile>structTourmaline_1_1Containers_1_1HashContainerOptions.html</anchorfile>
      <anchor>a7d07d2abaaa908c1f439d8949e23dc3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>leaningFactor</name>
      <anchorfile>structTourmaline_1_1Containers_1_1HashContainerOptions.html</anchorfile>
      <anchor>ab4fc57acc30a7142340c086eb58e823a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::size_t</type>
      <name>minimumBucketCount</name>
      <anchorfile>structTourmaline_1_1Containers_1_1HashContainerOptions.html</anchorfile>
      <anchor>adac5afe01d43520bfecde2bb5efe5576</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::size_t</type>
      <name>reservedBucketSpace</name>
      <anchorfile>structTourmaline_1_1Containers_1_1HashContainerOptions.html</anchorfile>
      <anchor>ac0784da346113d0fc439738d24a6cd61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Containers::Hashlist</name>
    <filename>classTourmaline_1_1Containers_1_1Hashlist.html</filename>
    <templarg>Concepts::Hashable Entry</templarg>
    <member kind="function">
      <type></type>
      <name>Hashlist</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashlist.html</anchorfile>
      <anchor>a6b896ab2522a83bbac01eb204e2d688b</anchor>
      <arglist>(HashContainerOptions options={})</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Insert</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashlist.html</anchorfile>
      <anchor>ae935341aac7c2ccc31f1f2634f255672</anchor>
      <arglist>(Entry entry)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Remove</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashlist.html</anchorfile>
      <anchor>a9f53d4a00138d744b7ef9e8b4df2d474</anchor>
      <arglist>(const Entry &amp;entry)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Has</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashlist.html</anchorfile>
      <anchor>ae990c1d85a84ab0d004af8f1c61b44c9</anchor>
      <arglist>(const Entry &amp;entry) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; Entry &gt;</type>
      <name>ExtractAllEntries</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashlist.html</anchorfile>
      <anchor>a111d08bf77d86ad0296b82161deccc15</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; Entry &gt;</type>
      <name>ListAllEntries</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashlist.html</anchorfile>
      <anchor>a6449c11dce90eed91c69f45f874c3012</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Clear</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashlist.html</anchorfile>
      <anchor>af54354dc9523c0463800778874609321</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>Count</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashlist.html</anchorfile>
      <anchor>a28a044943def2e865932491c3136661e</anchor>
      <arglist>() noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Containers::Hashmap</name>
    <filename>classTourmaline_1_1Containers_1_1Hashmap.html</filename>
    <templarg>Concepts::Hashable Key</templarg>
    <templarg>typename Value</templarg>
    <member kind="function">
      <type></type>
      <name>Hashmap</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>a06e6b73accf568025a94bc9143da1aea</anchor>
      <arglist>(HashContainerOptions options={})</arglist>
    </member>
    <member kind="function">
      <type>Value &amp;</type>
      <name>Insert</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>a02d08052423af502984bb21d90fee602</anchor>
      <arglist>(Key key, Value value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Remove</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>acb419af54253e9c84be860f289b5c4f8</anchor>
      <arglist>(const Key &amp;key)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Has</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>a6b281575fa4611856f791ff9d7dffe05</anchor>
      <arglist>(const Key &amp;key) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>Value &amp;</type>
      <name>Get</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>ac71eceaef5196a208064c14655a54acf</anchor>
      <arglist>(const Key &amp;key)</arglist>
    </member>
    <member kind="function">
      <type>const Value &amp;</type>
      <name>GetView</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>a7c657e55b03b629de3fc5242ecc446bc</anchor>
      <arglist>(const Key &amp;key) const</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; Value &gt;</type>
      <name>ExtractAllValues</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>a541c0557f6f2afdb1ff3be7e2a2fe9bc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; Value &gt;</type>
      <name>ListAllValues</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>ad49b3af198566bb3747d9b7936160682</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; Key &gt;</type>
      <name>ListAllKeys</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>a8456205ccdf71617311a4a828e33a1b5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Clear</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>a3525f4bbd0bfe012c445688ba2203f8a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>Count</name>
      <anchorfile>classTourmaline_1_1Containers_1_1Hashmap.html</anchorfile>
      <anchor>abccfad07a8f0137dc5fb6310692365ac</anchor>
      <arglist>() noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Systems::Importer</name>
    <filename>classTourmaline_1_1Systems_1_1Importer.html</filename>
    <class kind="struct">Tourmaline::Systems::Importer::MeshInfo</class>
    <member kind="function" static="yes">
      <type>static Magnum::Trade::ImageData2D</type>
      <name>LoadImage2D</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Importer.html</anchorfile>
      <anchor>a2f360d549e62c7d772218f9125bb2d33</anchor>
      <arglist>(Corrade::Containers::StringView path)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Magnum::GL::Texture2D</type>
      <name>LoadTexture2D</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Importer.html</anchorfile>
      <anchor>a483e169c865f5456b9edd92d7bb15f37</anchor>
      <arglist>(Magnum::Trade::ImageData2D image, Magnum::GL::SamplerFilter filterType=Magnum::GL::SamplerFilter::Nearest, Magnum::GL::SamplerWrapping wrappingRule=Magnum::GL::SamplerWrapping::ClampToEdge)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Magnum::GL::Texture2D</type>
      <name>LoadTexture2D</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Importer.html</anchorfile>
      <anchor>a5b02d2791312e44683053a0604834b52</anchor>
      <arglist>(Corrade::Containers::StringView path, Magnum::GL::SamplerFilter filterType=Magnum::GL::SamplerFilter::Nearest, Magnum::GL::SamplerWrapping wrappingRule=Magnum::GL::SamplerWrapping::ClampToEdge)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Magnum::GL::Mesh</type>
      <name>LoadMesh</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Importer.html</anchorfile>
      <anchor>ae60a6640b1cd4ed4631886d2039af922</anchor>
      <arglist>(Corrade::Containers::StringView path, uint64_t index=0)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Magnum::GL::Mesh</type>
      <name>LoadMesh</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Importer.html</anchorfile>
      <anchor>aefce9d1f868c439992871e81c668a51f</anchor>
      <arglist>(Corrade::Containers::StringView path, Corrade::Containers::StringView name)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::vector&lt; MeshInfo &gt;</type>
      <name>LoadAllMeshes</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Importer.html</anchorfile>
      <anchor>a68e92d2572a48d76338d534a77a50d95</anchor>
      <arglist>(Corrade::Containers::StringView path)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Game::Input::Key</name>
    <filename>classTourmaline_1_1Game_1_1Input_1_1Key.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>IsInactive</name>
      <anchorfile>classTourmaline_1_1Game_1_1Input_1_1Key.html</anchorfile>
      <anchor>a5534ca01ce2fcc1a74cab88a5bcfb97d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsActive</name>
      <anchorfile>classTourmaline_1_1Game_1_1Input_1_1Key.html</anchorfile>
      <anchor>ab6c00994f655b37fc3e94e07021181a6</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsDown</name>
      <anchorfile>classTourmaline_1_1Game_1_1Input_1_1Key.html</anchorfile>
      <anchor>aa0e17cd9992ee522c7f5cdf995ed3c0e</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPressed</name>
      <anchorfile>classTourmaline_1_1Game_1_1Input_1_1Key.html</anchorfile>
      <anchor>a5c92ec31889f2055874de1a5a1a493e7</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsReleased</name>
      <anchorfile>classTourmaline_1_1Game_1_1Input_1_1Key.html</anchorfile>
      <anchor>a0dbc3c8b684769c5b22f00bc3b4a55af</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsHeld</name>
      <anchorfile>classTourmaline_1_1Game_1_1Input_1_1Key.html</anchorfile>
      <anchor>a5010697f389066bafb8ce13d49e76b00</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="variable">
      <type>KeyState</type>
      <name>state</name>
      <anchorfile>classTourmaline_1_1Game_1_1Input_1_1Key.html</anchorfile>
      <anchor>a86824ae5bb4cbedbbe1edadf33f28956</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>KeyType</type>
      <name>type</name>
      <anchorfile>classTourmaline_1_1Game_1_1Input_1_1Key.html</anchorfile>
      <anchor>a401ad1e043a9a6e6e9d5a1663d9649b8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Systems::Logging</name>
    <filename>classTourmaline_1_1Systems_1_1Logging.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>LogLevel</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Logging.html</anchorfile>
      <anchor>ab2f520eaef4ada2bb62d40763becb691</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>LogToFile</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Logging.html</anchorfile>
      <anchor>a3cd8f7fe5b027e8615b3810e011c635b</anchor>
      <arglist>(Corrade::Containers::String File=&quot;&quot;)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>Log</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Logging.html</anchorfile>
      <anchor>ab679a2d42e7bbde6c127b595dbab22cb</anchor>
      <arglist>(Corrade::Containers::StringView message, Corrade::Containers::StringView position=&quot;Unknown&quot;, LogLevel severity=LogLevel::Info, bool condition=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>LogFormatted</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Logging.html</anchorfile>
      <anchor>afddcb6b02db70d31e4206802a8f8bfa6</anchor>
      <arglist>(const char *format, const char *position, LogLevel severity, const Args &amp;...args)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static Corrade::Containers::Function&lt; void()&gt;</type>
      <name>TerminationFunction</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Logging.html</anchorfile>
      <anchor>ab9000641fb8e46a2fc7f12043dfbc547</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Type::Matrix</name>
    <filename>classTourmaline_1_1Type_1_1Matrix.html</filename>
    <base>Magnum::Math::Matrix4&lt; float &gt;</base>
    <member kind="function">
      <type></type>
      <name>Matrix</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>ace3641b35d9f6eee33fdf1cf5b3eaabf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Matrix</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>ad110b501fef3e5a6e2ea16059c5aa345</anchor>
      <arglist>(float fillWith)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Matrix</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a9ce89208d89dca504694b03f8c1ab328</anchor>
      <arglist>(const Magnum::Math::Matrix4&lt; float &gt; &amp;copy)</arglist>
    </member>
    <member kind="function">
      <type>Matrix &amp;</type>
      <name>operator=</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a0ed27c968a0f7df62a9875923aa2e57a</anchor>
      <arglist>(const Magnum::Math::Matrix4&lt; float &gt; &amp;copy)</arglist>
    </member>
    <member kind="function">
      <type>Matrix &amp;</type>
      <name>translate</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>add7fd3588c7eb3ee4bb906cd6b7a3f90</anchor>
      <arglist>(const Magnum::Math::Vector3&lt; float &gt; &amp;offset, Apply order=Apply::Locally)</arglist>
    </member>
    <member kind="function">
      <type>Matrix &amp;</type>
      <name>setTranslation</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>ac1f7da5354b8e30b1d87ab16c7303b49</anchor>
      <arglist>(const Magnum::Math::Vector3&lt; float &gt; &amp;position)</arglist>
    </member>
    <member kind="function">
      <type>Matrix &amp;</type>
      <name>rotate</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a4679e350b67c7686bc64896d553d09e2</anchor>
      <arglist>(float angle, Magnum::Math::Vector3&lt; float &gt; rotationAxis, Apply order=Apply::Locally, AngleUnit anglesIn=AngleUnit::Radiants)</arglist>
    </member>
    <member kind="function">
      <type>Matrix &amp;</type>
      <name>rotateQuaternion</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a843823ce728520fd8a1716e7ee763c28</anchor>
      <arglist>(Magnum::Math::Quaternion&lt; float &gt; quaternion, Apply order=Apply::Locally)</arglist>
    </member>
    <member kind="function">
      <type>Matrix &amp;</type>
      <name>rotateEuler</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a18214f24664251df7fa9d7df5ba3b4d7</anchor>
      <arglist>(const Magnum::Math::Vector3&lt; float &gt; &amp;angles, Apply order=Apply::Locally, AngleUnit anglesIn=AngleUnit::Radiants)</arglist>
    </member>
    <member kind="function">
      <type>Magnum::Math::Vector3&lt; float &gt;</type>
      <name>getEulerAngles</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>aac1613c35881482e1f587e961edca916</anchor>
      <arglist>(AngleUnit anglesIn=AngleUnit::Radiants) const</arglist>
    </member>
    <member kind="function">
      <type>Magnum::Math::Quaternion&lt; float &gt;</type>
      <name>getQuaternion</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a7d5f432ce34ffd17e24b2ceb25237150</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Matrix &amp;</type>
      <name>scale</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>ab1a6635677fe04c7ff2c8be9029b859f</anchor>
      <arglist>(const Magnum::Math::Vector3&lt; float &gt; &amp;scaleBy, Apply order=Apply::Locally)</arglist>
    </member>
    <member kind="function">
      <type>Matrix &amp;</type>
      <name>scale</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a9fec6d18b171c5f3dc0604a95b37e8cb</anchor>
      <arglist>(float scaleBy, Apply order=Apply::Locally)</arglist>
    </member>
    <member kind="function">
      <type>Matrix &amp;</type>
      <name>setScaling</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a0a949a993901ad2164c585166944df96</anchor>
      <arglist>(const Magnum::Math::Vector3&lt; float &gt; &amp;scale)</arglist>
    </member>
    <member kind="function">
      <type>Matrix &amp;</type>
      <name>setScaling</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a34b2ab00414e27be7575319543967fa4</anchor>
      <arglist>(float scale)</arglist>
    </member>
    <member kind="function">
      <type>Magnum::Math::Matrix4&lt; float &gt; &amp;</type>
      <name>asMagnumMatrix</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a0138460f3f128998f79833869066a7bf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Matrix</type>
      <name>FromTranslation</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a158a1a79de41842f3ad3f269a6129863</anchor>
      <arglist>(const Magnum::Math::Vector3&lt; float &gt; &amp;translation)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr uint8_t</type>
      <name>Rows</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a3bbc42a0c41711508cbd4caac14d16ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr uint8_t</type>
      <name>Columns</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>af0b79f9f16cde3d508099ab67cba5c72</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr uint8_t</type>
      <name>Elements</name>
      <anchorfile>classTourmaline_1_1Type_1_1Matrix.html</anchorfile>
      <anchor>a53c29094a538e1fcd1f40676665640bb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Systems::Importer::MeshInfo</name>
    <filename>structTourmaline_1_1Systems_1_1Importer_1_1MeshInfo.html</filename>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Containers::DualkeyMap::MultiQueryResult</name>
    <filename>structTourmaline_1_1Containers_1_1DualkeyMap_1_1MultiQueryResult.html</filename>
    <templarg>typename OppositeKey</templarg>
    <member kind="variable">
      <type>const OppositeKey *</type>
      <name>oppositeKey</name>
      <anchorfile>structTourmaline_1_1Containers_1_1DualkeyMap_1_1MultiQueryResult.html</anchorfile>
      <anchor>ae84c4085cc3ed9991b40ba2129804fc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Corrade::Containers::Array&lt; Value * &gt;</type>
      <name>valueQueryResults</name>
      <anchorfile>structTourmaline_1_1Containers_1_1DualkeyMap_1_1MultiQueryResult.html</anchorfile>
      <anchor>adea1e9d98693959c494a0740b762b8ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::size_t</type>
      <name>howManyFound</name>
      <anchorfile>structTourmaline_1_1Containers_1_1DualkeyMap_1_1MultiQueryResult.html</anchorfile>
      <anchor>a59ff6cf54a29a5366f881947e1101ad9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Game::Input::Pointer</name>
    <filename>structTourmaline_1_1Game_1_1Input_1_1Pointer.html</filename>
    <member kind="function">
      <type>PointerButtonState</type>
      <name>GetButtonState</name>
      <anchorfile>structTourmaline_1_1Game_1_1Input_1_1Pointer.html</anchorfile>
      <anchor>a46155324c2798dbcccac2f9fd456abd0</anchor>
      <arglist>(PointerButtonType button) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInactive</name>
      <anchorfile>structTourmaline_1_1Game_1_1Input_1_1Pointer.html</anchorfile>
      <anchor>ac2251e133051a9e823b250bd4fe1c148</anchor>
      <arglist>(PointerButtonType button) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsActive</name>
      <anchorfile>structTourmaline_1_1Game_1_1Input_1_1Pointer.html</anchorfile>
      <anchor>a25bcb6d17513b0c7b941038df3ce499d</anchor>
      <arglist>(PointerButtonType button) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsDown</name>
      <anchorfile>structTourmaline_1_1Game_1_1Input_1_1Pointer.html</anchorfile>
      <anchor>afd2652cf6fcef8135c3968456f03e752</anchor>
      <arglist>(PointerButtonType button) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPressed</name>
      <anchorfile>structTourmaline_1_1Game_1_1Input_1_1Pointer.html</anchorfile>
      <anchor>a3fd7dde45ac7f9ece1fa3fd3679e5cc3</anchor>
      <arglist>(PointerButtonType button) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsReleased</name>
      <anchorfile>structTourmaline_1_1Game_1_1Input_1_1Pointer.html</anchorfile>
      <anchor>ae96a96cace91777f2107685f2e5b172d</anchor>
      <arglist>(PointerButtonType button) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsHeld</name>
      <anchorfile>structTourmaline_1_1Game_1_1Input_1_1Pointer.html</anchorfile>
      <anchor>ad513dc12d585d6a387436c43e0142bf3</anchor>
      <arglist>(PointerButtonType button) const noexcept</arglist>
    </member>
    <member kind="variable">
      <type>Magnum::Vector2i</type>
      <name>position</name>
      <anchorfile>structTourmaline_1_1Game_1_1Input_1_1Pointer.html</anchorfile>
      <anchor>abcb4e925351fd87388dafe0a0a1d18f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Magnum::Vector2</type>
      <name>scrollAmount</name>
      <anchorfile>structTourmaline_1_1Game_1_1Input_1_1Pointer.html</anchorfile>
      <anchor>a6741ef8913536c55cf9af71b3e10fc68</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Systems::ECS::Prefab</name>
    <filename>classTourmaline_1_1Systems_1_1ECS_1_1Prefab.html</filename>
    <templarg>isAComponent... Components</templarg>
    <member kind="typedef">
      <type>decltype(components)</type>
      <name>tupleSignature</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1Prefab.html</anchorfile>
      <anchor>ac2c022434fd6039fa22e42ff5a22a3b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Prefab</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1Prefab.html</anchorfile>
      <anchor>a7dc1e63207a565321425f8105ff36012</anchor>
      <arglist>(Components... arguments)</arglist>
    </member>
    <member kind="function">
      <type>std::tuple&lt; Components... &gt; &amp;</type>
      <name>GetTuple</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1Prefab.html</anchorfile>
      <anchor>abfb3ec8196b1012a7eb7c9c9078d33fa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Component &amp;</type>
      <name>GetComponent</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1Prefab.html</anchorfile>
      <anchor>a9841b7f9fd6e60193a2a5541393ab78e</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Game::Program</name>
    <filename>classTourmaline_1_1Game_1_1Program.html</filename>
    <class kind="struct">Tourmaline::Game::Program::Args</class>
    <class kind="struct">Tourmaline::Game::Program::Config</class>
    <member kind="function">
      <type>int</type>
      <name>Run</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>a56808b449b5d92f4e83b335262addf61</anchor>
      <arglist>(const Config &amp;conf)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ApplyNewConfig</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>a97e48f57a87f3685c67325e012c468ab</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>OnStart</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>a84163b75ebd97764e1b49c2e5f0258cc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>OnStep</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>a8cb2dda44a37a71bcb71c72e14bbeaad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>OnCrash</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>a8738d0770ee6fcc915b78af4532bc607</anchor>
      <arglist>(const std::exception &amp;exception)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>OnExit</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>ace439c9dd63d598a819d4449cc79d250</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Input::Key &amp;</type>
      <name>GetKey</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>adf07e7204a4f5549758c6ad7ca80b632</anchor>
      <arglist>(const Input::KeyType &amp;keyType)</arglist>
    </member>
    <member kind="function">
      <type>const Input::Pointer &amp;</type>
      <name>GetMouse</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>a178f58f5545b69b139427650583f1e59</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>Config</type>
      <name>config</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>a40db18b543cb536158bfb0c4e3b79adb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Systems::ECS::World</type>
      <name>World</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>aa6f2ace81f0510f41cf5343bbcd855e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const float &amp;</type>
      <name>deltaTime</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>ad8f688cad6cc1b3fd2a76d25d4f787f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const float &amp;</type>
      <name>aspectRatio</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>ac4f218819ca19814826efc1b15b0a272</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static Args</type>
      <name>arguments</name>
      <anchorfile>classTourmaline_1_1Game_1_1Program.html</anchorfile>
      <anchor>a948e2b1144c95ff0880a7cbee116a010</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Systems::Random</name>
    <filename>classTourmaline_1_1Systems_1_1Random.html</filename>
    <member kind="function" static="yes">
      <type>static T</type>
      <name>Generate</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Random.html</anchorfile>
      <anchor>ad7f0d789525d5681a7f67fba6b4a92e7</anchor>
      <arglist>(T max, T min=0)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Tourmaline::Type::UUID</type>
      <name>GenerateUUID</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Random.html</anchorfile>
      <anchor>a5940dcd23d92aecc7a769634001f1dd1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>SetSeed</name>
      <anchorfile>classTourmaline_1_1Systems_1_1Random.html</anchorfile>
      <anchor>a18320f47f89c11a3cf50fb95b5f6ce6f</anchor>
      <arglist>(uint64_t seed)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Systems::Serialization::Serializable</name>
    <filename>structTourmaline_1_1Systems_1_1Serialization_1_1Serializable.html</filename>
    <templarg>typename Type</templarg>
    <class kind="struct">Tourmaline::Systems::Serialization::Serializable::SerialList</class>
    <member kind="function">
      <type>std::string</type>
      <name>Serialize</name>
      <anchorfile>structTourmaline_1_1Systems_1_1Serialization_1_1Serializable.html</anchorfile>
      <anchor>ac244025a3544484db53d9786cfcfe13a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Deserialize</name>
      <anchorfile>structTourmaline_1_1Systems_1_1Serialization_1_1Serializable.html</anchorfile>
      <anchor>a25eba679c23212f535f9dd00ba39a721</anchor>
      <arglist>(std::string_view serialData)</arglist>
    </member>
    <member kind="function">
      <type>Type &amp;</type>
      <name>getTypeInstance</name>
      <anchorfile>structTourmaline_1_1Systems_1_1Serialization_1_1Serializable.html</anchorfile>
      <anchor>a814de2d7c3ad7839b7ec88240a6bb0cd</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Systems::Serialization::Serializable::SerialList</name>
    <filename>structTourmaline_1_1Systems_1_1Serialization_1_1Serializable_1_1SerialList.html</filename>
    <templarg>typename... dataType</templarg>
    <base>std::tuple&lt; dataType Type::*... &gt;</base>
    <member kind="function">
      <type></type>
      <name>SerialList</name>
      <anchorfile>structTourmaline_1_1Systems_1_1Serialization_1_1Serializable_1_1SerialList.html</anchorfile>
      <anchor>aeada68e349f0b361fbfde35ffb124dd5</anchor>
      <arglist>(dataType Type::*...memberPointers)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr std::size_t</type>
      <name>Count</name>
      <anchorfile>structTourmaline_1_1Systems_1_1Serialization_1_1Serializable_1_1SerialList.html</anchorfile>
      <anchor>ac22a8ee88f9a787c625b9a412a8d9cd0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Stats</name>
    <filename>structStats.html</filename>
    <base>Tourmaline::Systems::ECS::Component</base>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Systems::Components::Transform</name>
    <filename>structTourmaline_1_1Systems_1_1Components_1_1Transform.html</filename>
    <base>Tourmaline::Systems::ECS::Component</base>
    <member kind="function">
      <type></type>
      <name>Transform</name>
      <anchorfile>structTourmaline_1_1Systems_1_1Components_1_1Transform.html</anchorfile>
      <anchor>a181d95b857b25d406ab61bf6d3e39b4a</anchor>
      <arglist>(Type::Matrix matrix={})</arglist>
    </member>
    <member kind="variable">
      <type>Type::Matrix</type>
      <name>Matrix</name>
      <anchorfile>structTourmaline_1_1Systems_1_1Components_1_1Transform.html</anchorfile>
      <anchor>a42dff5c55efb09ba9090cf4347df4918</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tourmaline::Type::UnspecifiedType</name>
    <filename>structTourmaline_1_1Type_1_1UnspecifiedType.html</filename>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Type::UUID</name>
    <filename>classTourmaline_1_1Type_1_1UUID.html</filename>
    <member kind="function">
      <type>Corrade::Containers::String</type>
      <name>asString</name>
      <anchorfile>classTourmaline_1_1Type_1_1UUID.html</anchorfile>
      <anchor>a73ef06647f8bf8dddd52f9202a9ee51c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>UUID</name>
      <anchorfile>classTourmaline_1_1Type_1_1UUID.html</anchorfile>
      <anchor>a0ce157aa72c71629f56f8fe3666a155b</anchor>
      <arglist>(uint64_t firstHalf=0, uint64_t secondHalf=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>UUID</name>
      <anchorfile>classTourmaline_1_1Type_1_1UUID.html</anchorfile>
      <anchor>ab89880afd09a898bed59253d14702b38</anchor>
      <arglist>(const std::string &amp;uuid)</arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>firstHalf</name>
      <anchorfile>classTourmaline_1_1Type_1_1UUID.html</anchorfile>
      <anchor>a79967c426e5b207187c9e262b40af4fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>secondHalf</name>
      <anchorfile>classTourmaline_1_1Type_1_1UUID.html</anchorfile>
      <anchor>a19e064fdec7dd11d2e3fbb4c99725214</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const UUID</type>
      <name>Empty</name>
      <anchorfile>classTourmaline_1_1Type_1_1UUID.html</anchorfile>
      <anchor>a8790dfb31d56aa8c889be721847090ec</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tourmaline::Systems::ECS::World</name>
    <filename>classTourmaline_1_1Systems_1_1ECS_1_1World.html</filename>
    <member kind="function">
      <type></type>
      <name>World</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a48eac311fe56ebe0ec385a426f9743d0</anchor>
      <arglist>(uint64_t minimumEntryCount=20 &apos;000)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Step</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>ac9af60f4d2127f844372a889c7e6af36</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Entity</type>
      <name>CreateEntity</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a21db62be4521f263266d2c469427cb92</anchor>
      <arglist>(const Components::Transform &amp;transformMatrix, bool isEnabled=true, Type::UUID presetUUID=Type::UUID::Empty)</arglist>
    </member>
    <member kind="function">
      <type>Entity</type>
      <name>CreateEntity</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>ae5a0ec0416c6838ec07c6105aba0252a</anchor>
      <arglist>(bool isEnabled=true, Type::UUID presetUUID=Type::UUID::Empty)</arglist>
    </member>
    <member kind="function">
      <type>Entity</type>
      <name>CreateFromPrefab</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a6b6af21860573c71c15f5dc8ca3e30f7</anchor>
      <arglist>(const Prefab&lt; Components... &gt; &amp;prefab, bool isEnabled=true, Type::UUID presetUUID=Type::UUID::Empty)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EntityExists</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a0df8213f3624fecbaeca561bb78d858b</anchor>
      <arglist>(const Entity &amp;entity) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>DestroyEntity</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>ab400363e3b9edda776d6656603304e94</anchor>
      <arglist>(Entity entity)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetEntityEnable</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a147c73f34856a3371c79f0530bcb83a9</anchor>
      <arglist>(const Entity &amp;entity, bool beEnabled=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>GetEntityEnable</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>ac791b8209db02907163b211c45124622</anchor>
      <arglist>(const Entity &amp;entity) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetEntityLabel</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>aa1c85231dfa55709c3697adca095b2f1</anchor>
      <arglist>(const Entity &amp;entity, Corrade::Containers::String label)</arglist>
    </member>
    <member kind="function">
      <type>Corrade::Containers::StringView</type>
      <name>GetEntityLabel</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a352e831c128b82eff5bb5c18c04eea86</anchor>
      <arglist>(const Entity &amp;entity) noexcept</arglist>
    </member>
    <member kind="function">
      <type>System</type>
      <name>AddSystem</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a3e8c2ccdaf469d9235b03abaca7c2703</anchor>
      <arglist>(SystemFunction &amp;&amp;system, Instance *instance, SystemPriority priority=SystemPriority::Default, bool enabled=true)</arglist>
    </member>
    <member kind="function">
      <type>System</type>
      <name>AddSystem</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>ab94ff507399963bad7e355884e0f8282</anchor>
      <arglist>(SystemFunction &amp;&amp;system, SystemPriority priority=SystemPriority::Default, bool enabled=true, Instance *instance=nullptr)</arglist>
    </member>
    <member kind="function">
      <type>std::span&lt; std::vector&lt; System &gt; &gt;</type>
      <name>ListAllSystems</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>abfa32a5250b9f97c0bb59143b03a6358</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>GetSystemEnable</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a7e705050525a7e11445495c5091f42a5</anchor>
      <arglist>(const System &amp;system) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetSystemEnable</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a81955a33f79e5a61a024098c6d1218e1</anchor>
      <arglist>(const System &amp;system, bool beEnabled=true)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>InvokeSystem</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>ad23f960a9465988c4d7bea3ddb89e126</anchor>
      <arglist>(const System &amp;system, bool ignoreEnabled=true)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>RemoveSystem</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a6f5ba6de3b8676b5fd4f13674120944b</anchor>
      <arglist>(const System &amp;system)</arglist>
    </member>
    <member kind="function">
      <type>Component &amp;</type>
      <name>AddComponent</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>ad5df8b2f9d8a86613bed48919cfa4503</anchor>
      <arglist>(const Entity &amp;entity, ComponentArgs &amp;&amp;...args)</arglist>
    </member>
    <member kind="function">
      <type>Component &amp;</type>
      <name>GetComponent</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a44a358f87cf0dc1cc7e104e8025db86c</anchor>
      <arglist>(const Entity &amp;entity)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; std::pair&lt; const Entity &amp;, Component &amp; &gt; &gt; &amp;</type>
      <name>GetAllOfComponents</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a520eec51281c625062dd56397b4fe814</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>HasComponent</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a04b3511424cba33744ac9d8e74f91be1</anchor>
      <arglist>(const Entity &amp;entity)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>RemoveComponent</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>ad124e63a6da7b0ee64c11d0a293a3c0d</anchor>
      <arglist>(const Entity &amp;entity)</arglist>
    </member>
    <member kind="function">
      <type>Hook</type>
      <name>AddHook</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a7477b78b2b8c6a8af68cd484ce112b54</anchor>
      <arglist>(HookFunction &amp;&amp;hook, HookTo hookPosition, bool isEnabled=true, Instance *instance=nullptr)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>GetHookEnable</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a118b37bd32266ccc480e4c8515e12541</anchor>
      <arglist>(const Hook &amp;hook) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetHookEnable</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a015dcccec5990713ee0a0ddafea71ab9</anchor>
      <arglist>(const Hook &amp;hook, bool beEnabled=true)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>RemoveHook</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a9ad018ea1f8334e58a54183d9d5ac4f0</anchor>
      <arglist>(const Hook &amp;hook)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>World</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a461694e708db8135d227d520bd16da03</anchor>
      <arglist>(const World &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>World &amp;</type>
      <name>operator=</name>
      <anchorfile>classTourmaline_1_1Systems_1_1ECS_1_1World.html</anchorfile>
      <anchor>a9f7d2459048940b07209c274a397a96a</anchor>
      <arglist>(const World &amp;)=delete</arglist>
    </member>
  </compound>
  <compound kind="concept">
    <name>Tourmaline::Concepts::Either</name>
    <filename>conceptTourmaline_1_1Concepts_1_1Either.html</filename>
  </compound>
  <compound kind="concept">
    <name>Tourmaline::Concepts::Hashable</name>
    <filename>conceptTourmaline_1_1Concepts_1_1Hashable.html</filename>
  </compound>
  <compound kind="concept">
    <name>Tourmaline::Systems::ECS::isAComponent</name>
    <filename>conceptTourmaline_1_1Systems_1_1ECS_1_1isAComponent.html</filename>
  </compound>
  <compound kind="page">
    <name>01-Hello-Tourmaline</name>
    <title>01. Getting Started - Installing and Preparing Tourmaline</title>
    <filename>01-Hello-Tourmaline.html</filename>
  </compound>
  <compound kind="page">
    <name>02-Logging</name>
    <title>02. Logging - How to write log messages</title>
    <filename>02-Logging.html</filename>
  </compound>
  <compound kind="page">
    <name>03-Random</name>
    <title>03. Random - Random value generation</title>
    <filename>03-Random.html</filename>
  </compound>
  <compound kind="page">
    <name>04-ECS-Basics</name>
    <title>04. ECS Basics - Entities &amp; Components</title>
    <filename>04-ECS-Basics.html</filename>
    <subpage>ECS-Basics-Example.html</subpage>
  </compound>
  <compound kind="page">
    <name>examples</name>
    <title>Examples</title>
    <filename>examples.html</filename>
    <subpage>01-Hello-Tourmaline.html</subpage>
    <subpage>02-Logging.html</subpage>
    <subpage>03-Random.html</subpage>
    <subpage>04-ECS-Basics.html</subpage>
  </compound>
  <compound kind="page">
    <name>ECS-Basics-Example</name>
    <title>examples/04-ECS-Basics/main.cpp</title>
    <filename>ECS-Basics-Example.html</filename>
  </compound>
  <compound kind="page">
    <name>supportedFileTypes</name>
    <title>Supported File-types</title>
    <filename>supportedFileTypes.html</filename>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Info</title>
    <filename>index.html</filename>
  </compound>
</tagfile>
