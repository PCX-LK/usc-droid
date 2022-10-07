#include "stdafx.h"
#include "Mesh.hpp"
#include <Graphics/ResourceManagers.hpp>

#ifdef ANDROID

#define GL_GLEXT_PROTOTYPES
#include "SDL_opengles2.h"
#include <SDL_opengles2_gl2.h>
#include <SDL_opengles2_gl2ext.h>

#endif

namespace Graphics
{
	uint32 primitiveTypeMap[] =
	{
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN,
		GL_LINES,
		GL_LINE_STRIP,
		GL_POINTS,
	};
	class Mesh_Impl : public MeshRes
	{
		uint32 m_buffer = 0;
		uint32 m_vao = 0;
		PrimitiveType m_type;
		uint32 m_glType;
		size_t m_vertexCount;
		bool m_bDynamic = true;
	public:
		Mesh_Impl()
		{
		}
		~Mesh_Impl()
		{
			if(m_buffer)
				glDeleteBuffers(1, &m_buffer);
			if(m_vao)
#ifdef ANDROID
				glDeleteVertexArraysOES(1, &m_vao);
#else
				glDeleteVertexArrays(1, &m_vao);
#endif
		}
		bool Init()
		{
			glGenBuffers(1, &m_buffer);
#ifdef ANDROID
			glGenVertexArraysOES(1, &m_vao);
#else
			glGenVertexArrays(1, &m_vao);
#endif
			return m_buffer != 0 && m_vao != 0;
		}

		void SetData(const void* pData, size_t vertexCount, const VertexFormatList& desc) override
		{
#ifdef ANDROID
			glBindVertexArrayOES(m_vao);
#else
			glBindVertexArray(m_vao);
#endif
			glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

			m_vertexCount = vertexCount;
			size_t totalVertexSize = 0;
			for(auto e : desc)
				totalVertexSize += e.componentSize * e.components;
			size_t index = 0;
			size_t offset = 0;
			for(auto e : desc)
			{
				uint32 type = -1;
				if(!e.isFloat)
				{
					if(e.componentSize == 4)
						type = e.isSigned ? GL_INT : GL_UNSIGNED_INT;
					else if(e.componentSize == 2)
						type = e.isSigned ? GL_SHORT : GL_UNSIGNED_SHORT;
					else if(e.componentSize == 1)
						type = e.isSigned ? GL_BYTE : GL_UNSIGNED_BYTE;
				}
				else
				{
					#ifdef EMBEDDED
					type = GL_FLOAT;
					#else
					if(e.componentSize == 4)
						type = GL_FLOAT;
					else if(e.componentSize == 8)
						type = GL_DOUBLE;
					#endif
				}
				assert(type != (uint32)-1);
				glVertexAttribPointer((int)index, (int)e.components, type, GL_TRUE, (int)totalVertexSize, (void*)offset);
				glEnableVertexAttribArray((int)index);
				offset += e.componentSize * e.components;
				index++;
			}
			glBufferData(GL_ARRAY_BUFFER, totalVertexSize * vertexCount, pData, m_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

#ifdef ANDROID
			glBindVertexArrayOES(0);
#else
			glBindVertexArray(0);
#endif
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		
		#ifdef EMBEDDED
		void Draw() override
		{
#ifdef ANDROID
			glBindVertexArrayOES(m_vao);
#else
			glBindVertexArray(m_vao);	
#endif
			
			glDrawArrays(m_glType, 0, (int)m_vertexCount);
			
#ifdef ANDROID
			glBindVertexArrayOES(0);
#else
			glBindVertexArray(0);
#endif
		}
		void Redraw() override
		{
#ifdef ANDROID
			glBindVertexArrayOES(m_vao);
#else
			glBindVertexArray(m_vao);
#endif

			glDrawArrays(m_glType, 0, (int)m_vertexCount);

#ifdef ANDROID
			glBindVertexArrayOES(0);
#else
			glBindVertexArray(0);
#endif
		}
		#else
		void Draw() override
		{
			glBindVertexArray(m_vao);
			glDrawArrays(m_glType, 0, (int)m_vertexCount);
		}
		void Redraw() override
		{
			glDrawArrays(m_glType, 0, (int)m_vertexCount);
		}
		#endif

		void SetPrimitiveType(PrimitiveType pt) override
		{
			m_type = pt;
			m_glType = primitiveTypeMap[(size_t)pt];
		}
		virtual PrimitiveType GetPrimitiveType() const override
		{
			return m_type;
		}
	};

	Mesh MeshRes::Create(class OpenGL* gl)
	{
		Mesh_Impl* pImpl = new Mesh_Impl();
		if(!pImpl->Init())
		{
			delete pImpl;
			return Mesh();
		}
		else
		{
			return GetResourceManager<ResourceType::Mesh>().Register(pImpl);
		}
	}
}