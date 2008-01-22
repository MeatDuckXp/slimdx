/*
* Copyright (c) 2007 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <d3d10.h>
#include <d3dx10.h>

#include "DXGIErrorHandler.h"

#include "SwapChain.h"
#include "FrameStatistics.h"
#include "ModeDescription.h"

namespace SlimDX
{
namespace DXGI
{ 	
	generic< class T > where T : SlimDXObject, ref class
	T SwapChain::GetBuffer( int index )
	{
		IUnknown* unknown = 0;
		GUID guid = Utilities::GetNativeGuidForType( T::typeid );
		HRESULT hr = InternalPointer->GetBuffer( index, guid, reinterpret_cast<void**>( &unknown ) );
		if( DXGIErrorHandler::TestForFailure( hr ) )
			return T();
		return safe_cast<T>( Activator::CreateInstance( T::typeid, IntPtr( unknown ) ) );
	}

	FrameStatistics SwapChain::GetFrameStatistics()
	{
		DXGI_FRAME_STATISTICS stats;
		HRESULT hr = InternalPointer->GetFrameStatistics( &stats );
		if( DXGIErrorHandler::TestForFailure( hr ) )
			return FrameStatistics();
		return FrameStatistics( stats );
	}

	void SwapChain::ResizeBuffers( int count, int width, int height, SlimDX::DXGI::Format format, SwapChainFlags flags )
	{
		HRESULT hr = InternalPointer->ResizeBuffers( count, width, height, static_cast<DXGI_FORMAT>( format ), static_cast<UINT>( flags ) );
		DXGIErrorHandler::TestForFailure( hr );
	}
	
	void SwapChain::ResizeTarget( ModeDescription description )
	{
		HRESULT hr = InternalPointer->ResizeTarget( reinterpret_cast<DXGI_MODE_DESC*>( &description ) );
		DXGIErrorHandler::TestForFailure( hr );
	}

	PresentResult SwapChain::Present( int syncInterval, PresentFlags flags )
	{
		HRESULT hr = InternalPointer->Present( syncInterval, static_cast<UINT>( flags ) );
		if( hr == S_OK )
			return PresentResult::Okay;
		else if( hr == DXGI_STATUS_OCCLUDED )
			return PresentResult::Occluded;
		
		DXGIErrorHandler::TestForFailure( hr );
		return PresentResult::Failed;
	}
}
}