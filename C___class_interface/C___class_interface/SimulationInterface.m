%EXAMPLE_INTERFACE Example MATLAB class wrapper to an underlying C++ class
classdef SimulationInterface < handle
    properties (SetAccess = private, Hidden = true)
        objectHandle; % Handle to the underlying C++ class instance
    end
    methods
        %% Constructor - Create a new C++ class instance 
        function this = SimulationInterface(varargin)
            this.objectHandle = control('new', varargin{:});
        end
        
        %% Destructor - Destroy the C++ class instance
        function delete(this)
            % Call mex function with the name of the source cpp file
            control('delete', this.objectHandle);
        end

        %% Train - an example class method call
        function varargout = initSimulation(this, varargin)
            % Call mex function with the name of the source cpp file
            [varargout{1:nargout}] = control('initSimulation', this.objectHandle, varargin{:});
        end

        %% Test - another example class method call
        function varargout = runForDayInMatlab(this, varargin)
            % Call mex function with the name of the source cpp file
            [varargout{1:nargout}] = control('runForDayInMatlab', this.objectHandle, varargin{:});
        end
    end
end