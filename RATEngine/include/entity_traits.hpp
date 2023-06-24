#pragma once

namespace rat
{
	class IHasSolidity
	{
	public:
		virtual bool IsSolid() { return is_solid; }
		virtual void SetSolidity(bool value) { is_solid = value; }
		virtual void SwapSolidity() { is_solid = !is_solid; }

	protected:
		bool is_solid;
	};

	class IHasOpacity
	{
	public:
		virtual bool IsOpaque() { return is_opaque; }
		virtual void SetOpacity(bool value) { is_opaque = value; }
		virtual void SwapOpacity() { is_opaque = !is_opaque; }

	protected:
		bool is_opaque;
	};
}

