#pragma once

namespace softlit
{
	typedef std::vector<glm::vec3> VertexBuffer;
	typedef std::vector<uint64_t> IndexBuffer;

#define MAX_ATTRIBUTES_VEC4 3
#define MAX_ATTRIBUTES_VEC3 3
#define MAX_ATTRIBUTES_VEC2 3

	/*
	vec-N only vertex attributes that can be fed into vertex shaders
	*/
	template<int N>
	struct AttributeBuffer
	{
		std::vector<glm::vec<N, float>> m_data; // Buffer of actual data
		std::vector<uint64_t> m_index; // Buffer of index into data

		const glm::vec<N, float>& operator[](size_t i) const
		{
			return m_data[m_index[i]];
		}
	};

	struct VertexAttributes
	{
		std::vector<AttributeBuffer<4>> attrib_vec4;
		std::vector<AttributeBuffer<3>> attrib_vec3;
		std::vector<AttributeBuffer<2>> attrib_vec2;

		VertexAttributes()
		{
			attrib_vec4.reserve(MAX_ATTRIBUTES_VEC4);
			attrib_vec3.reserve(MAX_ATTRIBUTES_VEC3);
			attrib_vec2.reserve(MAX_ATTRIBUTES_VEC2);
		}
	};

	// Vertex attributes to be passed down to VS/FS after getting fetched & populated
	typedef struct Attribute
	{
		std::vector<glm::vec4> attrib_vec4;
		std::vector<glm::vec3> attrib_vec3;
		std::vector<glm::vec2> attrib_vec2;

		Attribute()
		{
			attrib_vec4.reserve(MAX_ATTRIBUTES_VEC4);
			attrib_vec3.reserve(MAX_ATTRIBUTES_VEC3);
			attrib_vec2.reserve(MAX_ATTRIBUTES_VEC2);
		}

		void ResetData()
		{
			attrib_vec4.clear();
			attrib_vec3.clear();
			attrib_vec2.clear();
		}

		template<int N>
		void PushVertexAttribute(const glm::vec<N, float>& v)
		{
			static_assert(N < 4);
		}

		template<>
		void PushVertexAttribute(const glm::vec4& v)
		{
			attrib_vec4.push_back(v);
		}

		template<>
		void PushVertexAttribute(const glm::vec3& v)
		{
			attrib_vec3.push_back(v);
		}

		template<>
		void PushVertexAttribute(const glm::vec2& v)
		{
			attrib_vec2.push_back(v);
		}
	}Vertex_IN, Vertex_OUT;

	// Per-primitive buffer object
	typedef void* UniformBuffer;

	// vertex_shader to be run per-vertex. returned vec4() is vertex converted to clip-coordinates
	typedef glm::vec4(*vertex_shader)(const glm::vec3& pos, const UniformBuffer ubo, const Vertex_IN* const in, Vertex_OUT* out);

	// fragment_shader to output final render target values
	typedef glm::vec4(*fragment_shader)(const UniformBuffer ubo, const Vertex_OUT* const in);

	class Primitive
	{
	public:
		Primitive(const PrimitiveSetup& setup);
		~Primitive();

		const VertexBuffer& getVertexBuffer() const { return m_vertexBuffer; }
		const IndexBuffer& getIndexBuffer() const { return m_indexBuffer; }

		void setVertexBuffer(const VertexBuffer& vb) { m_vertexBuffer = vb; }
		void setIndexBuffer(const IndexBuffer& ib) { m_indexBuffer = ib; }

		vertex_shader getVS() { return m_VS; }
		fragment_shader getFS() { return m_FS; }

		void setVS(const vertex_shader vs) { m_VS = vs; }
		void setFS(const fragment_shader fs) { m_FS = fs; }

		UniformBuffer UBO() { return m_ubo; }
		void UBO(UniformBuffer ubo) { m_ubo = ubo; }

		const VertexAttributes& getVertexAttributes() const { return m_attribs; }

		template<int N>
		void appendAttributeBuffer(const AttributeBuffer<N>& attributes)
		{
			static_assert(N < 4);
		}

		template<>
		void appendAttributeBuffer(const AttributeBuffer<4>& attributes)
		{
			m_attribs.attrib_vec4.push_back(attributes);
		}

		template<>
		void appendAttributeBuffer(const AttributeBuffer<3>& attributes)
		{
			m_attribs.attrib_vec3.push_back(attributes);
		}

		template<>
		void appendAttributeBuffer(const AttributeBuffer<2>& attributes)
		{
			m_attribs.attrib_vec2.push_back(attributes);
		}

		const PrimitiveSetup& getPrimitiveSetup() const { return m_setup; }

	private:
		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;

		UniformBuffer m_ubo = nullptr;
		vertex_shader m_VS = nullptr;
		fragment_shader m_FS = nullptr;

		VertexAttributes m_attribs;

		PrimitiveSetup m_setup;
	};
}